SELECT
    t.name                     AS team_name,
    tr.name                    AS team_role,
    COUNT(DISTINCT tm.user_id) AS member_count
FROM team t
    CROSS JOIN team_member tm
    CROSS JOIN team_role tr
GROUP BY
    t.team_id,
    t.name,
    tr.team_role_id,
    tr.name
ORDER BY
    t.name,
    tr.name
LIMIT 15;

-- cross join
-- ограничить число команд в запросе
