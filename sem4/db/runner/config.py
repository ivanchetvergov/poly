QUERIES = [
    {
        "name": "q01",
        "description": (
            "Соревнования где пользователь A в роли B "
            "сделал попытку и соревнование содержит датасет C"
        ),
        "sql_file": "q01.sql",
        "param_queries":  """
            SELECT u.username, r.name, d.name
            FROM participation p
            JOIN "user" u               ON u.user_id          = p.user_id
            JOIN role r                 ON r.role_id          = u.role_id
            JOIN submission s           ON s.participation_id = p.participation_id
            JOIN competition_dataset cd ON cd.competition_id  = p.competition_id
            JOIN dataset d              ON d.dataset_id       = cd.dataset_id
            ORDER BY RANDOM()
            LIMIT 1
        """,
        "retries": 10,
        "on_empty": "warn",
        "plot": None,
    },
    {
        "name": "q02",
        "description": (
            "Число соревнований для метрики A, команды B и датасета C"
        ),
        "sql_file": "q02.sql",
        "param_queries": """
            SELECT m.name, t.name, d.name
            FROM team t
            JOIN team_competition tc    ON tc.team_id        = t.team_id
            JOIN dataset d              ON d.dataset_id      = tc.dataset_id
            JOIN competition c          ON c.competition_id  = t.competition_id
            JOIN configuration cfg      ON cfg.competition_id = c.competition_id
            JOIN metric m               ON m.metric_id       = cfg.metric_id
            JOIN competition_dataset cd ON cd.competition_id = c.competition_id
                                        AND cd.dataset_id    = tc.dataset_id
            ORDER BY RANDOM()
            LIMIT 1
        """,
        "retries": 10,
        "on_empty": "warn",
        "plot": None,
    },
    {
        "name": "q03",
        "description": "Первые 20 команд — число участий и соревнований",
        "sql_file": "q03.sql",
        "param_queries": [],
        "retries": 1,
        "on_empty": "warn",
        "plot": "q03_histogram",
    },
    {
        "name": "q04",
        "description": "Пользователи с максимальным числом участий",
        "sql_file": "q04.sql",
        "param_queries": [],
        "retries": 1,
        "on_empty": "warn",
        "plot": None,
    },
    {
        "name": "q05",
        "description": "Число пользователей с одинаковым числом участий в командах",
        "sql_file": "q05.sql",
        "param_queries": [],
        "retries": 1,
        "on_empty": "warn",
        "plot": "q05_2d",
    },
]
