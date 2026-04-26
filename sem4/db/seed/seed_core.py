import random
from faker import Faker


TEAM_PREFIXES = [
    "Gradient",
    "Boosted",
    "Silent",
    "Vector",
    "Kernel",
    "Bayes",
    "Quantum",
    "Spark",
]

TEAM_SUFFIXES = [
    "Otters",
    "Falcons",
    "Pirates",
    "Wizards",
    "Racers",
    "Nomads",
    "Monkeys",
    "Giants",
]


async def seed_participations(inserter, count: int = 50) -> int:
    return await inserter.seed(
        table='participation',
        query=(
            'INSERT INTO participation (user_id, competition_id, team_id, status_id)\n'
            'VALUES ($1, $2, $3, $4)\n'
            'ON CONFLICT (user_id, competition_id) DO NOTHING'
        ),
        generator=lambda deps: (
            random.choice(deps['users']),
            int((team_pair := random.choice(deps['team_competitions'])).split(':', 1)[1]),
            int(team_pair.split(':', 1)[0]),
            random.choice(deps['status']),
        ),
        dependencies={
            'users': 'SELECT user_id FROM "user"',
            'team_competitions': "SELECT team_id::text || ':' || competition_id::text FROM team",
            'status': 'SELECT status_id FROM participation_status',
        },
        count=count,
    )

async def seed_configurations(inserter, count: int = 10) -> int:
    return await inserter.seed(
        table='configuration',
        query=(
            'INSERT INTO configuration (metric_id, task_type_id, competition_id, daily_attempt_limit)\n'
            'VALUES ($1, $2, $3, $4)\n'
            'ON CONFLICT (competition_id, metric_id, task_type_id) DO NOTHING'
        ),
        generator=lambda deps: (
            random.choice(deps['metrics']),
            random.choice(deps['task_types']),
            random.choice(deps['competitions']),
            random.randint(1, 100),
        ),
        dependencies={
            'metrics': 'SELECT metric_id FROM metric',
            'task_types': 'SELECT task_type_id FROM task_type',
            'competitions': 'SELECT competition_id FROM competition',
        },
        count=count,
    )

async def seed_competition_datasets(
    inserter,
    min_per_competition: int = 1,
    max_per_competition: int = 3,
) -> int:
    if min_per_competition > max_per_competition:
        raise ValueError('min_per_competition cannot be greater than max_per_competition')

    competitions = [
        row[0]
        for row in await inserter.conn.fetch('SELECT competition_id FROM competition')
    ]
    datasets = [
        row[0]
        for row in await inserter.conn.fetch('SELECT dataset_id FROM dataset')
    ]

    if not competitions or not datasets:
        print('Skip competition_dataset: no competitions or datasets.')
        return 0

    existing_rows = await inserter.conn.fetch(
        'SELECT competition_id, dataset_id FROM competition_dataset'
    )
    by_competition: dict[int, set[int]] = {comp_id: set() for comp_id in competitions}
    existing_dataset_ids: set[int] = set()

    for row in existing_rows:
        comp_id = int(row[0])
        ds_id = int(row[1])
        if comp_id in by_competition:
            by_competition[comp_id].add(ds_id)
        existing_dataset_ids.add(ds_id)

    insert_rows: list[tuple[int, int]] = []

    for comp_id in competitions:
        assigned = by_competition[comp_id]
        target = random.randint(min_per_competition, max_per_competition)
        needed = max(0, target - len(assigned))
        if needed <= 0:
            continue

        available = [dataset_id for dataset_id in datasets if dataset_id not in assigned]
        if not available:
            continue

        for dataset_id in random.sample(available, min(needed, len(available))):
            assigned.add(dataset_id)
            existing_dataset_ids.add(dataset_id)
            insert_rows.append((comp_id, dataset_id))

    missing_datasets = [dataset_id for dataset_id in datasets if dataset_id not in existing_dataset_ids]
    for dataset_id in missing_datasets:
        comp_id = random.choice(competitions)
        if dataset_id not in by_competition[comp_id]:
            by_competition[comp_id].add(dataset_id)
            insert_rows.append((comp_id, dataset_id))

    if not insert_rows:
        print('Insert 0 items into competition_dataset.')
        return 0

    await inserter.conn.executemany(
        (
            'INSERT INTO competition_dataset (competition_id, dataset_id)\n'
            'VALUES ($1, $2)\n'
            'ON CONFLICT (competition_id, dataset_id) DO NOTHING'
        ),
        insert_rows,
    )
    print(f'Insert {len(insert_rows)} items into competition_dataset.')
    return len(insert_rows)

async def seed_teams(
    inserter,
    fake: Faker,
    min_per_competition: int = 3,
    max_per_competition: int = 15,
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
            f"{random.choice(TEAM_PREFIXES)} {random.choice(TEAM_SUFFIXES)} {random.randint(1, 99)}"[:30],
            random.choice(deps['status']),
        ),
        dependencies={
            'competition': 'SELECT competition_id FROM competition',
            'status': 'SELECT status_id FROM team_status',
        },
        per_dependency='competition',
        min_per_dependency=min_per_competition,
        max_per_dependency=max_per_competition,
    )

async def seed_team_members(
    inserter,
    min_per_team: int = 2,
    max_per_team: int = 5,
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
    min_per_team: int = 1,
    max_per_team: int = 2,
) -> int:
    return await inserter.seed(
        table='team_competition',
        query=(
            'INSERT INTO team_competition (team_id, dataset_id)\n'
            'VALUES ($1, $2)\n'
            'ON CONFLICT (team_id, dataset_id) DO NOTHING'
        ),
        generator=lambda deps: (
            deps.get('current_id'),
            random.choice(deps['dataset']),
        ),
        dependencies={
            'team': 'SELECT team_id FROM team',
            'dataset': 'SELECT dataset_id FROM dataset',
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
