SELECT
    (
        SELECT SUM(sub.submission_count)
        FROM (
            SELECT COUNT(DISTINCT s.submission_id) AS submission_count
            FROM (
                SELECT team_id, name
                FROM team
                ORDER BY team_id
                LIMIT 20
                ) t
            JOIN participation p ON p.team_id         = t.team_id
            JOIN submission s    ON s.participation_id = p.participation_id
            GROUP BY t.team_id
        ) sub
    )                                        AS sum_from_main,
    COUNT(DISTINCT s.submission_id)          AS direct_count
FROM (
    SELECT team_id
    FROM team
    ORDER BY team_id
    LIMIT 20
    ) t
    JOIN participation p ON p.team_id         = t.team_id
    JOIN submission s    ON s.participation_id = p.participation_id;
