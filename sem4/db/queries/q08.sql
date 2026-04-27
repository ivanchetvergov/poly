SELECT
    t.name                     AS team_name,
    tr.name                    AS team_role,
    COUNT(DISTINCT tm.user_id) AS member_count
FROM team t
    JOIN team_member tm ON tm.team_id      = t.team_id
    JOIN team_role tr   ON tr.team_role_id = tm.team_role_id
GROUP BY
    t.team_id,
    t.name,
    tr.team_role_id,
    tr.name
ORDER BY
    t.name,
    tr.name;
