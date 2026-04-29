SELECT
    SUM(user_count)  AS q05_total,
    (
        SELECT COUNT(DISTINCT user_id)
        FROM team_member
    )                AS actual_total,
    SUM(user_count) = (
        SELECT COUNT(DISTINCT user_id)
        FROM team_member
    )                AS is_valid
FROM (
    SELECT
        u.user_id,
        COUNT(tm.member_id) AS tm_count,
        COUNT(u.user_id)    AS user_count
    FROM "user" u
        JOIN team_member tm ON tm.user_id = u.user_id
    GROUP BY
        u.user_id
) counts;
