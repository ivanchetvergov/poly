-- =====================================================
-- Для первых 20 команд: число участников и суммарное
-- число участий в соревнованиях через этих участников
-- =====================================================

SELECT
    t.name                       AS team_name,
    COUNT(DISTINCT tm.user_id)   AS member_count,
    COUNT(p.participation_id)    AS participation_count
FROM (
    SELECT team_id, name
    FROM team
    ORDER BY team_id
    LIMIT 20
) t
    JOIN team_member tm ON tm.team_id = t.team_id
    JOIN participation p ON p.user_id = tm.user_id
GROUP BY
    t.team_id,
    t.name
ORDER BY
    t.team_id;
