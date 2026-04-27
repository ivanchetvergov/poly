import random
from faker import Faker

from seed.constants import TEAM_NAMES, TEAM_SUFFIXES
from seed.settings import (
    COMPETITION_DATASETS_MAX,
    COMPETITION_DATASETS_MIN,
    TEAM_COMPETITIONS_MAX,
    TEAM_COMPETITIONS_MIN,
    TEAM_MEMBERS_MAX,
    TEAM_MEMBERS_MIN,
    TEAMS_PER_COMPETITION_MAX,
    TEAMS_PER_COMPETITION_MIN,
)


def _pick_competition_dataset(deps):
    team_id, competition_id = deps['current_id']
    matching_datasets = [
        dataset_id
        for comp_id, dataset_id in deps['competition_datasets']
        if comp_id == competition_id
    ]
    if not matching_datasets:
        matching_datasets = [dataset_id for _, dataset_id in deps['competition_datasets']]
    return team_id, random.choice(matching_datasets)


async def seed_participations(inserter, count: int = 50) -> int:
    return await inserter.seed(
        table='participation',
        query=(
            'INSERT INTO participation (user_id, competition_id, team_id, status_id, best_score, rank)\n'
            'VALUES ($1, $2, $3, $4, $5, $6)\n'
            'ON CONFLICT (user_id, competition_id) DO NOTHING'
        ),
        generator=lambda deps: (
            deps.get('current_id')[0],
            deps.get('current_id')[1],
            deps.get('current_id')[2],
            random.choice(deps['status']),
            None,
            None,
        ),
        dependencies={
            'team_members': (
                'SELECT DISTINCT ON (tm.user_id, t.competition_id) '
                'tm.user_id, t.competition_id, tm.team_id\n'
                'FROM team_member tm\n'
                'JOIN team t ON t.team_id = tm.team_id\n'
                'LEFT JOIN participation p '
                'ON p.user_id = tm.user_id AND p.competition_id = t.competition_id\n'
                'WHERE p.participation_id IS NULL\n'
                'ORDER BY tm.user_id, t.competition_id, tm.member_id'
            ),
            'status': 'SELECT status_id FROM participation_status',
        },
        per_dependency='team_members',
        min_per_dependency=1,
        max_per_dependency=1,
    )

async def seed_configurations(inserter) -> int:
    return await inserter.seed(
        table='configuration',
        query=(
            'INSERT INTO configuration (metric_id, task_type_id, competition_id, daily_attempt_limit)\n'
            'VALUES ($1, $2, $3, $4)\n'
            'ON CONFLICT (competition_id, metric_id, task_type_id) DO NOTHING'
        ),
        generator=lambda deps: (
            deps['metrics'][deps.get('current_id') % len(deps['metrics'])],
            deps['task_types'][deps.get('current_id') % len(deps['task_types'])],
            deps.get('current_id'),
            random.randint(12, 48),
        ),
        dependencies={
            'competitions': (
                'SELECT c.competition_id\n'
                'FROM competition c\n'
                'LEFT JOIN configuration cfg ON cfg.competition_id = c.competition_id\n'
                'WHERE cfg.competition_id IS NULL\n'
                'ORDER BY c.competition_id'
            ),
            'metrics': 'SELECT metric_id FROM metric',
            'task_types': 'SELECT task_type_id FROM task_type',
        },
        per_dependency='competitions',
        min_per_dependency=1,
        max_per_dependency=1,
    )

async def seed_competition_datasets(
    inserter,
    min_per_competition: int = COMPETITION_DATASETS_MIN,
    max_per_competition: int = COMPETITION_DATASETS_MAX,
) -> int:
    return await inserter.seed(
        table='competition_dataset',
        query=(
            'INSERT INTO competition_dataset (competition_id, dataset_id)\n'
            'VALUES ($1, $2)\n'
            'ON CONFLICT (competition_id, dataset_id) DO NOTHING'
        ),
        generator=lambda deps: (
            deps.get('current_id'),
            random.choice(deps['dataset']),
        ),
        dependencies={
            'competition': 'SELECT competition_id FROM competition',
            'dataset': 'SELECT dataset_id FROM dataset',
        },
        per_dependency='competition',
        min_per_dependency=min_per_competition,
        max_per_dependency=max_per_competition,
    )

async def seed_teams(
    inserter,
    fake: Faker,
    min_per_competition: int = TEAMS_PER_COMPETITION_MIN,
    max_per_competition: int = TEAMS_PER_COMPETITION_MAX,
) -> int:
    return await inserter.seed(
        table='team',
        query=(
            'INSERT INTO team (competition_id, name, status_id)\n'
            'VALUES ($1, $2, $3)\n'
            'ON CONFLICT (competition_id, name) DO NOTHING'
        ),
        generator=lambda deps: (
            deps.get('current_id'),
            f"{random.choice(TEAM_NAMES)} {random.choice(TEAM_SUFFIXES)} {deps['current_ordinal']}"[:30],
            random.choice(deps['status']),
        ),
        dependencies={
            'competition': (
                'SELECT c.competition_id\n'
                'FROM competition c\n'
                'LEFT JOIN team t ON t.competition_id = c.competition_id\n'
                'GROUP BY c.competition_id\n'
                'HAVING COUNT(t.team_id) = 0\n'
                'ORDER BY c.competition_id'
            ),
            'status': 'SELECT status_id FROM team_status',
        },
        per_dependency='competition',
        min_per_dependency=min_per_competition,
        max_per_dependency=max_per_competition,
    )

async def seed_team_members(
    inserter,
    min_per_team: int = TEAM_MEMBERS_MIN,
    max_per_team: int = TEAM_MEMBERS_MAX,
) -> int:
    return await inserter.seed(
        table='team_member',
        query=(
            'INSERT INTO team_member (team_id, user_id, team_role_id)\n'
            'VALUES ($1, $2, $3)'
        ),
        generator=lambda deps: (
            deps.get('current_id'),
            random.choice(deps['users']),
            random.choice(deps['roles']),
        ),
        dependencies={
            'team': 'SELECT team_id FROM team',
            'users': 'SELECT user_id FROM "user"',
            'roles': 'SELECT team_role_id FROM team_role',
        },
        per_dependency='team',
        min_per_dependency=min_per_team,
        max_per_dependency=max_per_team,
    )

async def seed_team_competitions(
    inserter,
    min_per_team: int = TEAM_COMPETITIONS_MIN,
    max_per_team: int = TEAM_COMPETITIONS_MAX,
) -> int:
    return await inserter.seed(
        table='team_competition',
        query=(
            'INSERT INTO team_competition (team_id, dataset_id)\n'
            'VALUES ($1, $2)\n'
            'ON CONFLICT (team_id, dataset_id) DO NOTHING'
        ),
        generator=_pick_competition_dataset,
        dependencies={
            'team': 'SELECT team_id, competition_id FROM team',
            'competition_datasets': 'SELECT competition_id, dataset_id FROM competition_dataset',
        },
        per_dependency='team',
        min_per_dependency=min_per_team,
        max_per_dependency=max_per_team,
    )

async def run_level2(inserter, fake: Faker) -> None:
    await seed_configurations(inserter)
    await seed_competition_datasets(inserter)
    await seed_teams(inserter, fake)
    await seed_team_members(inserter)
    await seed_team_competitions(inserter)
    await seed_participations(inserter)
