import asyncio
import asyncpg
from pathlib import Path
from dotenv import load_dotenv
import os

from .config import QUERIES
from .query_runner import QueryRunner

load_dotenv(Path(__file__).parent.parent / ".env")

DSN = (
    f"postgresql://{os.getenv('POSTGRES_USER')}"
    f":{os.getenv('POSTGRES_PASSWORD')}"
    f"@{os.getenv('DB_HOST')}"
    f":{os.getenv('DB_PORT')}"
    f"/{os.getenv('POSTGRES_DB')}"
)


async def main() -> None:
    conn = await asyncpg.connect(DSN)
    try:
        runner = QueryRunner(conn)
        for query_config in QUERIES:
            await runner.run(**query_config)
    finally:
        await conn.close()


if __name__ == "__main__":
    asyncio.run(main())
