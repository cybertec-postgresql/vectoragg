#include "postgres.h"
#include "fmgr.h"
#include "utils/array.h"
#include "catalog/pg_type_d.h"

#define ARRNELEMS(x)  ArrayGetNItems(ARR_NDIM(x), ARR_DIMS(x))

PG_MODULE_MAGIC;

static ArrayType *new_float8ArrayType(int num);
static ArrayType *new_float4ArrayType(int num);

PG_FUNCTION_INFO_V1(array_sum_float8);
Datum
array_sum_float8(PG_FUNCTION_ARGS)
{
	ArrayType  *v1 = PG_GETARG_ARRAYTYPE_P(0);
	int n = ARRNELEMS(v1);
	double *data = (double*) ARR_DATA_PTR(v1);

	double tstart = PG_GETARG_FLOAT8(1);
	double tend = PG_GETARG_FLOAT8(2);
	double tstep = (tend-tstart) /  (n-1);
	double astart = PG_GETARG_FLOAT8(3);
	double aend = PG_GETARG_FLOAT8(4);
	double result = 0.0;
	int i, istart, iend;

	if (ARR_HASNULL(v1))
		elog(ERROR, "Array has null entries");

	istart = (astart <= tstart) ? 0 : (astart > tend) ? n : (int) ((astart - tstart)/tstep);
	iend = (aend > tend) ? n : (aend <= tstart) ? 0 : (int) ((aend - tstart)/tstep);

	for (i = istart; i < iend; i++) {
		result += data[i];
	}

    PG_RETURN_FLOAT8(result);
}

static ArrayType *
new_float8ArrayType(int num)
{
	ArrayType  *r;
	int			nbytes;

	/* if no elements, return a zero-dimensional array */
	if (num <= 0)
	{
		r = construct_empty_array(FLOAT8OID);
		return r;
	}

	nbytes = ARR_OVERHEAD_NONULLS(1) + sizeof(double) * num;

	r = (ArrayType *) palloc0(nbytes);

	SET_VARSIZE(r, nbytes);
	ARR_NDIM(r) = 1;
	r->dataoffset = 0;			/* marker for no null bitmap */
	ARR_ELEMTYPE(r) = FLOAT8OID;
	ARR_DIMS(r)[0] = num;
	ARR_LBOUND(r)[0] = 1;

	return r;
}

PG_FUNCTION_INFO_V1(array_clamp_float8);
Datum
array_clamp_float8(PG_FUNCTION_ARGS)
{
	ArrayType  *v1 = PG_GETARG_ARRAYTYPE_P(0);
	int n = ARRNELEMS(v1);
	double *data = (double*) ARR_DATA_PTR(v1);

	double tstart = PG_GETARG_FLOAT8(1);
	double tend = PG_GETARG_FLOAT8(2);
	double tstep = (tend-tstart) /  (n-1);
	double astart = PG_GETARG_FLOAT8(3);
	double aend = PG_GETARG_FLOAT8(4);
	ArrayType *result;
	double *new_data;

	int i, istart, iend, j;

	if (ARR_HASNULL(v1))
		elog(ERROR, "Array has null entries");

	istart = (astart <= tstart) ? 0 : (astart > tend) ? n : (int) ((astart - tstart)/tstep);
	iend = (aend > tend) ? n : (aend <= tstart) ? 0 : (int) ((aend - tstart)/tstep);

	if (istart == 0 && iend == n)
		PG_RETURN_POINTER(v1);

	result = new_float8ArrayType(iend - istart);
	new_data = (double*) ARR_DATA_PTR(result);

	for (i = istart, j = 0; i < iend; i++, j++) {
		new_data[j] = data[i];
	}

	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(array_decimate_float8);
Datum
array_decimate_float8(PG_FUNCTION_ARGS)
{
	ArrayType  *v1 = PG_GETARG_ARRAYTYPE_P(0);
	int n = ARRNELEMS(v1);
	int new_n = (n+9)/10;
	double *data = (double*) ARR_DATA_PTR(v1);
	ArrayType *result = new_float8ArrayType(new_n);
	double *new_data = (double*) ARR_DATA_PTR(result);
	int i, j, k;

	for (i = 0, j = 0; i < n-9; i+=10, j++) {
		for (k = 0; k < 10; k++) {
			new_data[j] += 0.1*data[i+k];
		}
	}
	while (i < n) {
		new_data[j] += 0.1*data[i];
	}

	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(array_hundreth_float8);
Datum
array_hundreth_float8(PG_FUNCTION_ARGS)
{
	ArrayType  *v1 = PG_GETARG_ARRAYTYPE_P(0);
	int n = ARRNELEMS(v1);
	int new_n = (n+99)/100;
	double *data = (double*) ARR_DATA_PTR(v1);
	ArrayType *result = new_float8ArrayType(new_n);
	double *new_data = (double*) ARR_DATA_PTR(result);
	int i, j, k;

	for (i = 0, j = 0; i < n-99; i+=100, j++) {
		for (k = 0; k < 100; k++) {
			new_data[j] += 0.01*data[i+k];
		}
	}
	while (i < n) {
		new_data[j] += 0.01*data[i];
	}

	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(array_sum_float4);
Datum
array_sum_float4(PG_FUNCTION_ARGS)
{
	ArrayType  *v1 = PG_GETARG_ARRAYTYPE_P(0);
	int n = ARRNELEMS(v1);
	float *data = (float*) ARR_DATA_PTR(v1);

	double tstart = PG_GETARG_FLOAT8(1);
	double tend = PG_GETARG_FLOAT8(2);
	double tstep = (tend-tstart) /  (n-1);
	double astart = PG_GETARG_FLOAT8(3);
	double aend = PG_GETARG_FLOAT8(4);
	float result = 0.0;
	int i, istart, iend;

	if (ARR_HASNULL(v1))
		elog(ERROR, "Array has null entries");

	istart = (astart <= tstart) ? 0 : (astart > tend) ? n : (int) ((astart - tstart)/tstep);
	iend = (aend > tend) ? n : (aend <= tstart) ? 0 : (int) ((aend - tstart)/tstep);

	for (i = istart; i < iend; i++) {
		result += data[i];
	}

    PG_RETURN_FLOAT8((double) result);
}

static ArrayType *
new_float4ArrayType(int num)
{
	ArrayType  *r;
	int			nbytes;

	/* if no elements, return a zero-dimensional array */
	if (num <= 0)
	{
		r = construct_empty_array(FLOAT4OID);
		return r;
	}

	nbytes = ARR_OVERHEAD_NONULLS(1) + sizeof(double) * num;

	r = (ArrayType *) palloc0(nbytes);

	SET_VARSIZE(r, nbytes);
	ARR_NDIM(r) = 1;
	r->dataoffset = 0;			/* marker for no null bitmap */
	ARR_ELEMTYPE(r) = FLOAT4OID;
	ARR_DIMS(r)[0] = num;
	ARR_LBOUND(r)[0] = 1;

	return r;
}

PG_FUNCTION_INFO_V1(array_decimate_float4);
Datum
array_decimate_float4(PG_FUNCTION_ARGS)
{
	ArrayType  *v1 = PG_GETARG_ARRAYTYPE_P(0);
	int n = ARRNELEMS(v1);
	int new_n = (n+9)/10;
	float *data = (float*) ARR_DATA_PTR(v1);
	ArrayType *result = new_float4ArrayType(new_n);
	float *new_data = (float*) ARR_DATA_PTR(result);
	int i, j, k;

	for (i = 0, j = 0; i < n-9; i+=10, j++) {
		for (k = 0; k < 10; k++) {
			new_data[j] += 0.1*data[i+k];
		}
	}
	while (i < n) {
		new_data[j] += 0.1*data[i];
	}

	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(array_hundreth_float4);
Datum
array_hundreth_float4(PG_FUNCTION_ARGS)
{
	ArrayType  *v1 = PG_GETARG_ARRAYTYPE_P(0);
	int n = ARRNELEMS(v1);
	int new_n = (n+99)/100;
	float *data = (float*) ARR_DATA_PTR(v1);
	ArrayType *result = new_float4ArrayType(new_n);
	float *new_data = (float*) ARR_DATA_PTR(result);
	int i, j, k;

	for (i = 0, j = 0; i < n-99; i+=100, j++) {
		for (k = 0; k < 100; k++) {
			new_data[j] += 0.01*data[i+k];
		}
	}
	while (i < n) {
		new_data[j] += 0.01*data[i];
	}

	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(array_clamp_float4);
Datum
array_clamp_float4(PG_FUNCTION_ARGS)
{
	ArrayType  *v1 = PG_GETARG_ARRAYTYPE_P(0);
	int n = ARRNELEMS(v1);
	float *data = (float*) ARR_DATA_PTR(v1);

	double tstart = PG_GETARG_FLOAT8(1);
	double tend = PG_GETARG_FLOAT8(2);
	double tstep = (tend-tstart) /  (n-1);
	double astart = PG_GETARG_FLOAT8(3);
	double aend = PG_GETARG_FLOAT8(4);
	ArrayType *result;
	float *new_data;

	int i, istart, iend, j;

	if (ARR_HASNULL(v1))
		elog(ERROR, "Array has null entries");

	istart = (astart <= tstart) ? 0 : (astart > tend) ? n : (int) ((astart - tstart)/tstep);
	iend = (aend > tend) ? n : (aend <= tstart) ? 0 : (int) ((aend - tstart)/tstep);

	if (istart == 0 && iend == n)
		PG_RETURN_POINTER(v1);

	result = new_float4ArrayType(iend - istart);
	new_data = (float*) ARR_DATA_PTR(result);

	for (i = istart, j = 0; i < iend; i++, j++) {
		new_data[j] = data[i];
	}

	PG_RETURN_POINTER(result);
}
