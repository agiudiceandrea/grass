#include <stdlib.h>
#include <string.h>
#include "global.h"

int add_line(struct dxf_file *dxf, struct Map_info *Map)
{
    int code;
    char layer[DXF_BUF_SIZE];
    int layer_flag = 0;		/* indicates if a layer name has been found */
    int xflag = 0;		/* indicates if a x value has been found */
    int yflag = 0;		/* indicates if a y value has been found */
    int zflag = 0;		/* indicates if a z value has been found */
    int arr_size = 0;

    strcpy(layer, UNIDENTIFIED_LAYER);

    zpnts[0] = zpnts[1] = 0.0;
    /* read in lines and process information until a 0 is read in */
    while ((code = dxf_get_code(dxf)) != 0) {
	if (code == -2)
	    return -1;

	switch (code) {
	case 8:		/* layer name */
	    if (!layer_flag && *dxf_buf) {
		if (flag_list) {
		    if (!is_layer_in_list(dxf_buf)) {
			add_layer_to_list(dxf_buf);
			print_layer(dxf_buf);
		    }
		    return 0;
		}
		/* skip if layers != NULL && (
		 * (flag_invert == 0 && is_layer_in_list == 0) ||
		 * (flag_invert == 1 && is_layer_in_list == 1)
		 * )
		 */
		if (layers && flag_invert == is_layer_in_list(dxf_buf))
		    return 0;
		strcpy(layer, dxf_buf);
		layer_flag = 1;
	    }
	    break;
	case 10:		/* start point x coordinate */
	    xpnts[arr_size] = atof(dxf_buf);
	    xflag = 1;
	    break;
	case 11:		/* end point x coordinate */
	    xpnts[arr_size] = atof(dxf_buf);
	    xflag = 1;
	    break;
	case 20:		/* start point y coordinate */
	    ypnts[arr_size] = atof(dxf_buf);
	    yflag = 1;
	    break;
	case 21:		/* end point y coordinate */
	    ypnts[arr_size] = atof(dxf_buf);
	    yflag = 1;
	    break;
	case 30:		/* start point z coordinate */
	    zpnts[arr_size] = atof(dxf_buf);
	    zflag = 1;
	    break;
	case 31:		/* end point z coordinate */
	    zpnts[arr_size] = atof(dxf_buf);
	    zflag = 1;
	    break;
	}

	/* read in first two points */
	if (xflag && yflag && zflag && arr_size < 2) {
	    arr_size++;
	    xflag = 0;
	    yflag = 0;
	    zflag = 0;
	}
    }

    if (arr_size == 2)	/* have both start and stop */
	write_line(Map, layer, arr_size);

    return 0;
}
