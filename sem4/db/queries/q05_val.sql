SELECT
    SUM(user_count) AS total_from_histogram,
    (SELECT COUNT(*) FROM "user") AS total_users,
    SUM(user_count) = (SELECT COUNT(*) FROM "user") AS is_valid
FROM (
    SELECT
        tm_count,
        COUNT(user_id) AS user_count
    FROM (
        SELECT
            u.user_id,
            COUNT(tm.member_id) AS tm_count
        FROM "user" u
        LEFT JOIN team_member tm ON tm.user_id = u.user_id
        GROUP BY u.user_id
    ) counts
    GROUP BY tm_count
) histogram;
