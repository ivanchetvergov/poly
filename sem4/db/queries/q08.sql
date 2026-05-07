SELECT
    t.name  AS team_name,
    tr.name AS team_role,
    COUNT(DISTINCT tm.user_id) AS member_count
FROM (
    SELECT team_id, name
    FROM team
    ORDER BY name
    LIMIT 15
) t
CROSS JOIN team_role tr
LEFT JOIN team_member tm
    ON  tm.team_id      = t.team_id
    AND tm.team_role_id = tr.team_role_id
GROUP BY
    t.team_id,
    t.name,
    tr.team_role_id,
    tr.name
ORDER BY
    t.name,
    tr.name;

-- cross join
-- ограничить число команд в запросе
