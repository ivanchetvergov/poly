import asyncio
import random
from pathlib import Path
from typing import Any

import asyncpg

from .plots import OUTPUT_DIR, build_histogram, build_q05_2d

QUERIES_DIR = Path(__file__).parent.parent / "queries"

PLOT_REGISTRY = {
    "q03_histogram": lambda rows: build_histogram(
        rows,
        output_path=OUTPUT_DIR / "q03_histogram.png",
        kde=False,
    ),
    "q05_2d": lambda rows: build_q05_2d(
        rows,
        output_path=OUTPUT_DIR / "q05_2d.png",
    ),
}


class QueryRunner:
    def __init__(self, conn: asyncpg.Connection, rng: random.Random | None = None):
        self.conn = conn
        self.rng  = rng or random.Random()

    async def _pick_param(self, query: str) -> Any:
        rows = await self.conn.fetch(query)
        if not rows:
            raise ValueError(f"нет данных для параметра: {query}")
        return self.rng.choice(rows)[0]

    async def _pick_params(self, param_queries: list[str] | str) -> list[Any]:
        if isinstance(param_queries, str):
            rows = await self.conn.fetch(param_queries)
            if not rows:
                raise ValueError("нет данных для связного запроса параметров")
            return list(self.rng.choice(rows).values())
        return [await self._pick_param(q) for q in param_queries]

    async def run(
        self,
        *,
        name: str,
        description: str,
        sql_file: str,
        param_queries: list[str] | str,
        retries: int = 5,
        on_empty: str = "warn",
        plot: str | None = None,
    ) -> list[asyncpg.Record] | None:

        query = (QUERIES_DIR / sql_file).read_text(encoding="utf-8")

        print(f"\n{'='*60}")
        print(f"запрос : {name}")
        print(f"описание: {description}")

        if not param_queries:
            rows = await self.conn.fetch(query)
            self._print_result(rows)
            self._maybe_plot(plot, rows)
            return rows

        for attempt in range(1, retries + 1):
            params = await self._pick_params(param_queries)
            print(f"[попытка {attempt}/{retries}] параметры: {params}")
            rows = await self.conn.fetch(query, *params)
            if rows:
                self._print_result(rows)
                self._maybe_plot(plot, rows)
                return rows
            print("  -> пустой результат, повтор...")

        print(f"[{on_empty}] пустой результат после {retries} попыток")
        if on_empty == "raise":
            raise RuntimeError(f"нет результата: {sql_file}")
        return None

    def _maybe_plot(self, plot: str | None, rows: list[asyncpg.Record]) -> None:
        if plot is None or not rows:
            return
        fn = PLOT_REGISTRY.get(plot)
        if fn is None:
            print(f"  [warn] неизвестный график: {plot}")
            return
        fn([dict(r) for r in rows])

    def _print_result(self, rows: list[asyncpg.Record]) -> None:
        if not rows:
            print("результат: пусто")
            return
        print(f"результат: {len(rows)} строк")
        for row in rows[:5]:
            print(" ", dict(row))
        if len(rows) > 5:
            print(f"  ... еще {len(rows) - 5} строк")
