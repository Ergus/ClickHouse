-- Regression test for NOT_FOUND_COLUMN_IN_BLOCK when a row policy uses a Bool column
-- that is not selected by a view, and optimize_move_to_prewhere is enabled.
-- The bug caused SourceStepWithFilter::updatePrewhereInfo to double-apply the row_level_filter
-- to an output_header that had already had it applied during construction.

DROP TABLE IF EXISTS test_04092_prices;
DROP VIEW  IF EXISTS test_04092_view;
DROP ROW POLICY IF EXISTS test_04092_rp ON test_04092_prices;

CREATE TABLE test_04092_prices
(
    ts    DateTime,
    price Float64,
    src   String,
    is_valid Bool DEFAULT true
)
ENGINE = MergeTree
ORDER BY (ts)
SETTINGS index_granularity = 8192;

INSERT INTO test_04092_prices VALUES ('2026-04-06 00:00:00', 1.5,  'feed', true);
INSERT INTO test_04092_prices VALUES ('2026-04-06 01:00:00', 0.0,  'feed', false);  -- filtered by row policy
INSERT INTO test_04092_prices VALUES ('2026-04-06 02:00:00', 2.5,  'manual', true);

-- View that does NOT expose is_valid
CREATE VIEW test_04092_view AS
    SELECT ts, price, src
    FROM test_04092_prices;

-- Row policy on the underlying table using the Bool column
CREATE ROW POLICY test_04092_rp ON test_04092_prices USING is_valid = true TO ALL;

-- This query used to throw NOT_FOUND_COLUMN_IN_BLOCK with optimize_move_to_prewhere=1
-- because SourceStepWithFilter::updatePrewhereInfo re-applied the row_level_filter DAG
-- to an output header that had already consumed `is_valid`.
SELECT price, src
FROM test_04092_view
WHERE src != 'manual'
ORDER BY ts
SETTINGS optimize_move_to_prewhere = 1;

DROP ROW POLICY test_04092_rp ON test_04092_prices;
DROP VIEW  test_04092_view;
DROP TABLE test_04092_prices;
