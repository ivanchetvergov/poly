SELECT
    tm_count,
    COUNT(user_id) AS user_count
FROM (
    SELECT
        u.user_id,
        COUNT(tm.member_id) AS tm_count
    FROM "user" u
    LEFT JOIN team_member tm ON tm.user_id = u.user_id -- был баг, поменял JOIN на LEFT JOIN
    GROUP BY
        u.user_id
) counts
GROUP BY
    tm_count
ORDER BY
    tm_count DESC;
