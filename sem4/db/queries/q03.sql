SELECT
    t.name                           AS team_name,
    COUNT(DISTINCT tm.user_id)       AS member_count,
    COUNT(DISTINCT p.competition_id) AS participation_count
FROM (
    SELECT team_id, name
    FROM team
    ORDER BY team_id
    LIMIT 20
) t
    JOIN team_member tm       ON tm.team_id = t.team_id
    LEFT JOIN participation p ON p.team_id  = t.team_id
GROUP BY
    t.team_id,
    t.name
ORDER BY
    t.team_id;
