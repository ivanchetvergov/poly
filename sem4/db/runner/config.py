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
            JOIN team_member tm         ON tm.team_id         = p.team_id
            JOIN "user" u               ON u.user_id          = tm.user_id
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
            JOIN participation p ON p.team_id = t.team_id
            JOIN competition c ON c.competition_id = p.competition_id
            JOIN configuration cfg ON cfg.competition_id = c.competition_id
            JOIN metric m ON m.metric_id = cfg.metric_id
            JOIN competition_dataset cd ON cd.competition_id = c.competition_id
            JOIN dataset d ON d.dataset_id = cd.dataset_id
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
        "plot": "q03",
    },
    {
        "name": "q04_01",
        "description": "Пользователи с максимальным числом участий",
        "sql_file": "q04_01.sql",
        "param_queries": [],
        "retries": 1,
        "on_empty": "warn",
        "plot": None,
    },
    {
        "name": "q04_02",
        "description": "Пользователи с минимальным числом участий",
        "sql_file": "q04_02.sql",
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
        "plot": "q05",
    },
    {
        "name": "q06",
        "description": "Команды, у которых число участий больше, чем у другой команды",
        "sql_file": "q06.sql",
        "param_queries": "SELECT t.name FROM team t",
        "retries": 3,
        "on_empty": "warn",
        "plot": None,
    },
    {
        "name": "q07",
        "description": "Пользователи, которые не отправили решение для заданного файла",
        "sql_file": "q07.sql",
        "param_queries": "SELECT fd.filename FROM dataset_file fd",
        "retries": 3,
        "on_empty": "warn",
        "plot": None,
    },
    {
        "name": "q08",
        "description": "Роли команд и число участников",
        "sql_file": "q08.sql",
        "param_queries": [],
        "retries": 1,
        "on_empty": "warn",
        "plot": "q08",
    },
    {
        "name": "q09",
        "type": "update",
        "description": "Обновить метрику попытки для команды A в соревновании B",
        "sql_file": "q09.sql",
        "param_queries": """
            SELECT
                s.submission_id,
                ROUND(s.metric_value * 1.1, 6),
                s.metric_value,
                t.name,
                c.title,
                s.attempt_number
            FROM submission s
                JOIN participation p  ON p.participation_id = s.participation_id
                JOIN team t           ON t.team_id          = p.team_id
                JOIN competition c    ON c.competition_id   = p.competition_id
            WHERE s.metric_value IS NOT NULL
            AND p.team_id IS NOT NULL
            ORDER BY RANDOM()
            LIMIT 1
        """,
        "retries": 5,
        "on_empty": "warn",
        "plot": None,
    },
]
