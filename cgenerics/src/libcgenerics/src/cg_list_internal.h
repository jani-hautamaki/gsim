//******************************{begin:header}******************************//
//           cgenerics - The C Library for Generic Data Structures          //
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://github.com/jani-hautamaki/gsim
//
//      Copyright (C) 2013-2015 Jani Hautamaki <jani.hautamaki@hotmail.com>
//
//      Licensed under the terms of GNU General Public License v3.
//
//      You should have received a copy of the GNU General Public License v3
//      along with this program as the file LICENSE.txt; if not, please see
//      http://www.gnu.org/licenses/gpl-3.0.html
//
//********************************{end:header}******************************//

#ifndef CG_LIST_INTERNAL_H
#define CG_LIST_INTERNAL_H

#include "cgenerics/cg_list.h"

#ifdef __cplusplus
extern "C" {
#endif

void *cg_list_remove(cg_list *list, cg_listentry *entry);
void cg_list_delete(cg_list *list, cg_listentry *entry);

cg_listentry *cg_list_insert_after(
	cg_list *list,
	cg_listentry *entry,
	void *itemptr
);

cg_listentry *cg_list_insert_before(
	cg_list *list,
	cg_listentry *entry,
	void *itemptr
);


#ifdef __cplusplus
} // extern "C"
#endif

#endif

