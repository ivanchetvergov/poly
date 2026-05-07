SELECT
    (
        SELECT SUM(competition_count)
        FROM (
            SELECT COUNT(DISTINCT p.competition_id) AS competition_count
            FROM (SELECT team_id FROM team ORDER BY team_id LIMIT 20) t
                JOIN participation p ON p.team_id = t.team_id
            GROUP BY t.team_id
        ) sub
    )                                    AS sum_from_main,
    COUNT(DISTINCT p.competition_id)     AS direct_count
FROM (SELECT team_id FROM team ORDER BY team_id LIMIT 20) t
    JOIN participation p ON p.team_id = t.team_id;
