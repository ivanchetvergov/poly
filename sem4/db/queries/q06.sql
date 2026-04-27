SELECT
    ref.team_name              AS team_a,
    ref.participation_count    AS score_a,
    t.name                     AS team_b,
    COUNT(p.participation_id)  AS score_b
FROM team t
    JOIN participation p ON p.team_id = t.team_id,
    (
        SELECT
            t2.name                    AS team_name,
            COUNT(p2.participation_id) AS participation_count
        FROM team t2
            JOIN participation p2 ON p2.team_id = t2.team_id
        WHERE t2.name = $1
        GROUP BY t2.team_id, t2.name
    ) ref
GROUP BY
    t.team_id,
    t.name,
    ref.team_name,
    ref.participation_count
HAVING COUNT(p.participation_id) > ref.participation_count
ORDER BY
    score_b DESC;
