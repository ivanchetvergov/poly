from contextlib import redirect_stderr, redirect_stdout
from dataclasses import dataclass, field
from io import StringIO

import asyncpg
from faker import Faker

from cli import bootstrap  # noqa: F401
from cli.action_dispatcher import execute_action
from infra.db_config import get_pg_connect_kwargs
from inserter import Inserter


@dataclass
class ActionOutcome:
    inserted: int | None = None
    logs: list[str] = field(default_factory=list)
    error: str | None = None


async def run_seed_action(action: str, params: dict) -> ActionOutcome:
    conn = await asyncpg.connect(**get_pg_connect_kwargs())
    fake = Faker()
    inserter = Inserter(conn)
    buffer = StringIO()

    try:
        try:
            with redirect_stdout(buffer), redirect_stderr(buffer):
                inserted = await execute_action(
                    action,
                    conn=conn,
                    inserter=inserter,
                    fake=fake,
                    **params,
                )
            error = None
        except Exception as exc:
            inserted = None
            error = str(exc)

        logs = [line for line in buffer.getvalue().splitlines() if line.strip()]
        return ActionOutcome(inserted=inserted, logs=logs, error=error)
    finally:
        await conn.close()
