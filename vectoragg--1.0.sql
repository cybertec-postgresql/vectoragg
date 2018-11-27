\echo Use "CREATE EXTENSION base36" to load this file. \quit
CREATE FUNCTION array_sum(samples float8[], tstart float8, tend float8, astart float8, aend float8)
RETURNS float8
AS '$libdir/vectoragg', 'array_sum_float8'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION array_clamp(samples float8[], tstart float8, tend float8, astart float8, aend float8)
RETURNS float8[]
AS '$libdir/vectoragg', 'array_clamp_float8'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION array_decimate(samples float8[])
RETURNS float8[]
AS '$libdir/vectoragg', 'array_decimate_float8'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION array_hundreth(samples float8[])
RETURNS float8[]
AS '$libdir/vectoragg', 'array_hundreth_float8'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION array_sum(samples float4[], tstart float8, tend float8, astart float8, aend float8)
RETURNS float8
AS '$libdir/vectoragg', 'array_sum_float4'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION array_clamp(samples float4[], tstart float8, tend float8, astart float8, aend float8)
RETURNS float4[]
AS '$libdir/vectoragg', 'array_clamp_float4'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION array_decimate(samples float4[])
RETURNS float4[]
AS '$libdir/vectoragg', 'array_decimate_float4'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION array_hundreth(samples float4[])
RETURNS float4[]
AS '$libdir/vectoragg', 'array_hundreth_float4'
LANGUAGE C IMMUTABLE STRICT;