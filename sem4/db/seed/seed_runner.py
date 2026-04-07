import asyncio
import random
from pathlib import Path
import sys

import asyncpg
from faker import Faker

ROOT_DIR = Path(__file__).resolve().parents[1]
if str(ROOT_DIR) not in sys.path:
    sys.path.append(str(ROOT_DIR))

from infra.db_config import get_pg_connect_kwargs
from inserter import Inserter
from seed_dict import run_all_dictionaries
from seed_base import (
    run_level1,
    seed_competitions,
    seed_dataset_files,
    seed_datasets,
    seed_users,
)
from seed_core import (
    run_level2,
    seed_competition_datasets,
    seed_configurations,
    seed_participations,
    seed_team_competitions,
    seed_team_members,
    seed_teams,
)

from seed_sub import (
    run_level3,
    seed_evaluations,
    seed_leaderboard_entries,
    seed_solution_codes,
    seed_submissions,
)


ACTION_SPECS = {
    "all": {"defaults": {}},
    "dict": {"defaults": {}},
    "level1": {"defaults": {}},
    "level2": {"defaults": {}},
    "level3": {"defaults": {}},
    "users": {"defaults": {"count": 50}},
    "datasets": {"defaults": {"count": 10}},
    "dataset_files": {"defaults": {"min_per_dataset": 3, "max_per_dataset": 5}},
    "competitions": {"defaults": {"count": 10}},
    "configurations": {"defaults": {"count": 10}},
    "competition_datasets": {
        "defaults": {"min_per_competition": 1, "max_per_competition": 3}
    },
    "teams": {"defaults": {"count": 20}},
    "team_members": {"defaults": {"min_per_team": 2, "max_per_team": 5}},
    "team_competitions": {"defaults": {"min_per_team": 1, "max_per_team": 2}},
    "participations": {"defaults": {"count": 50}},
    "submissions": {
        "defaults": {"min_per_participation": 1, "max_per_participation": 3}
    },
    "solution_codes": {"defaults": {}},
    "evaluations": {"defaults": {}},
    "leaderboard_entries": {"defaults": {}},
}


async def seed_all(
    conn,
    *,
    rng: random.Random | None = None,
    fake: Faker | None = None,
) -> None:
    fake = fake or Faker()
    inserter = Inserter(conn, rng=rng)

    await run_all_dictionaries(conn)
    await run_level1(inserter, fake)
    await run_level2(inserter, fake)
    await run_level3(inserter, fake)


async def run_action(action: str, **kwargs) -> int | None:
    conn = await asyncpg.connect(**get_pg_connect_kwargs())
    fake = Faker()
    inserter = Inserter(conn)
    try:
        if action == "all":
            await seed_all(conn)
            return None
        if action == "dict":
            await run_all_dictionaries(conn)
            return None
        if action == "level1":
            await run_level1(inserter, fake)
            return None
        if action == "level2":
            await run_level2(inserter, fake)
            return None
        if action == "level3":
            await run_level3(inserter, fake)
            return None
        if action == "users":
            return await seed_users(inserter, fake, **kwargs)
        if action == "datasets":
            return await seed_datasets(inserter, fake, **kwargs)
        if action == "dataset_files":
            return await seed_dataset_files(inserter, fake, **kwargs)
        if action == "competitions":
            return await seed_competitions(inserter, fake, **kwargs)
        if action == "configurations":
            return await seed_configurations(inserter, **kwargs)
        if action == "competition_datasets":
            return await seed_competition_datasets(inserter, **kwargs)
        if action == "teams":
            return await seed_teams(inserter, fake, **kwargs)
        if action == "team_members":
            return await seed_team_members(inserter, **kwargs)
        if action == "team_competitions":
            return await seed_team_competitions(inserter, **kwargs)
        if action == "participations":
            return await seed_participations(inserter, **kwargs)
        if action == "submissions":
            return await seed_submissions(inserter, fake, **kwargs)
        if action == "solution_codes":
            return await seed_solution_codes(inserter, fake)
        if action == "evaluations":
            return await seed_evaluations(inserter)
        if action == "leaderboard_entries":
            return await seed_leaderboard_entries(inserter)

        raise ValueError(f"Unknown action: {action}")
    finally:
        await conn.close()


async def main() -> None:
    conn = await asyncpg.connect(**get_pg_connect_kwargs())
    try:
        await seed_all(conn)
        print("\nFull seed completed.")
    finally:
        await conn.close()


if __name__ == "__main__":
    asyncio.run(main())
