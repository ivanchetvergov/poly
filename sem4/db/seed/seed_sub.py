import random
from faker import Faker


LANGUAGE_SNIPPETS = {
    'python': 'def solve(x):\n    return sorted(x)\n',
    'cpp': '#include <vector>\nint solve(std::vector<int> x){return (int)x.size();}\n',
    'java': 'class Main { static int solve(int[] x){ return x.length; } }\n',
}

SOLUTION_TAGS = [
    'baseline+feature-hash',
    'cv-ensemble',
    'catboost-tuned',
    'lightgbm-fast',
    'transformer-small',
    'blend-stacking',
    'rule-hybrid',
]


async def seed_submissions(
    inserter,
    fake: Faker,
    min_per_participation: int = 1,
    max_per_participation: int = 3,
) -> int:
    return await inserter.seed(
        table='submission',
        query=(
            'INSERT INTO submission (\n'
            '  participation_id, status_id, file_path, attempt_number,\n'
            '  metric_value, is_valid, error_log, source_code, language, solution_description\n'
            ')\n'
            'VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)\n'
            'ON CONFLICT (participation_id, attempt_number) DO NOTHING'
        ),
        generator=lambda deps: (
            deps.get('current_id'),
            random.choice(deps['status']),
            fake.file_path(depth=3, extension='zip'),
            deps.get('current_ordinal'),
            round(min(1.0, max(0.0, random.betavariate(2.2, 1.8))), 6),
            random.choice([True, True, True, False]),
            None,
            LANGUAGE_SNIPPETS[(lang := random.choice(['python', 'cpp', 'java']))],
            lang,
            f"{random.choice(SOLUTION_TAGS)} | notes: {fake.sentence(nb_words=6)}"[:200],
        ),
        dependencies={
            'participation': 'SELECT participation_id FROM participation',
            'status': 'SELECT status_id FROM submission_status',
        },
        per_dependency='participation',
        min_per_dependency=min_per_participation,
        max_per_dependency=max_per_participation,
    )

async def seed_participation_scores(inserter) -> int:
    await inserter.conn.execute(
        '''
        UPDATE participation p
        SET best_score = best.best_score
        FROM (
            SELECT participation_id, MAX(metric_value) AS best_score
            FROM submission
            WHERE is_valid = TRUE
            GROUP BY participation_id
        ) AS best
        WHERE p.participation_id = best.participation_id
        '''
    )

    await inserter.conn.execute(
        '''
        WITH ranked AS (
            SELECT
                participation_id,
                RANK() OVER (
                    PARTITION BY competition_id
                    ORDER BY best_score DESC NULLS LAST
                ) AS rnk
            FROM participation
            WHERE best_score IS NOT NULL
        )
        UPDATE participation p
        SET rank = ranked.rnk
        FROM ranked
        WHERE p.participation_id = ranked.participation_id
        '''
    )
    print('Updated participation.best_score and participation.rank from submission data.')
    return 0


async def run_level3(inserter, fake: Faker) -> None:
    await seed_submissions(inserter, fake)
    await seed_participation_scores(inserter)
