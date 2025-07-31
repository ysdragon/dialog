/*
 *
 * Copyright (c) 2025 Youssef Saeed <youssefelkholey@gmail.com>
 *
 */

#include "ring.h"

#include "osdialog/osdialog.h"

RING_FUNC(ring_dialog_message)
{
	if (RING_API_PARACOUNT != 3)
	{
		RING_API_ERROR(RING_API_MISS3PARA);
		return;
	}
	if (!RING_API_ISNUMBER(1) || !RING_API_ISNUMBER(2) || !RING_API_ISSTRING(3))
	{
		RING_API_ERROR(RING_API_BADPARATYPE);
		return;
	}

	osdialog_message_level level = (osdialog_message_level)RING_API_GETNUMBER(1);
	osdialog_message_buttons buttons = (osdialog_message_buttons)RING_API_GETNUMBER(2);
	const char *message = RING_API_GETSTRING(3);

	int result = osdialog_message(level, buttons, message);
	RING_API_RETNUMBER(result);
}

RING_FUNC(ring_dialog_prompt)
{
	if (RING_API_PARACOUNT != 3)
	{
		RING_API_ERROR(RING_API_MISS3PARA);
		return;
	}
	if (!RING_API_ISNUMBER(1) || !RING_API_ISSTRING(2) || !RING_API_ISSTRING(3))
	{
		RING_API_ERROR(RING_API_BADPARATYPE);
		return;
	}

	osdialog_message_level level = (osdialog_message_level)RING_API_GETNUMBER(1);
	const char *message = RING_API_GETSTRING(2);
	const char *text = RING_API_GETSTRING(3);

	char *result = osdialog_prompt(level, message, text);
	if (result)
	{
		RING_API_RETSTRING(result);
		OSDIALOG_FREE(result);
	}
	else
	{
		RING_API_RETSTRING("");
	}
}

RING_FUNC(ring_dialog_file)
{
	if (RING_API_PARACOUNT != 4)
	{
		RING_API_ERROR(RING_API_MISS4PARA);
		return;
	}
	if (!RING_API_ISNUMBER(1) || !RING_API_ISSTRING(2) || !RING_API_ISSTRING(3) || !RING_API_ISSTRING(4))
	{
		RING_API_ERROR(RING_API_BADPARATYPE);
		return;
	}

	osdialog_file_action action = (osdialog_file_action)RING_API_GETNUMBER(1);
	const char *dir = RING_API_GETSTRING(2);
	const char *filename = RING_API_GETSTRING(3);
	const char *filters_str = RING_API_GETSTRING(4);

	osdialog_filters *filters = NULL;
	if (strlen(filters_str) > 0)
	{
		filters = osdialog_filters_parse(filters_str);
	}

	char *result = osdialog_file(action, dir, filename, filters);

	if (filters)
	{
		osdialog_filters_free(filters);
	}

	if (result)
	{
		RING_API_RETSTRING(result);
		OSDIALOG_FREE(result);
	}
	else
	{
		RING_API_RETSTRING("");
	}
}

RING_FUNC(ring_dialog_color_picker)
{
	if (RING_API_PARACOUNT != 2)
	{
		RING_API_ERROR(RING_API_MISS2PARA);
		return;
	}
	if (!RING_API_ISLIST(1) || !RING_API_ISNUMBER(2))
	{
		RING_API_ERROR(RING_API_BADPARATYPE);
		return;
	}

	List *pList = RING_API_GETLIST(1);
	if (ring_list_getsize(pList) != 4)
	{
		RING_API_ERROR("Color parameter must be a list of 4 numbers [r,g,b,a]");
		return;
	}

	osdialog_color color;
	color.r = (uint8_t)ring_list_getdouble(pList, 1);
	color.g = (uint8_t)ring_list_getdouble(pList, 2);
	color.b = (uint8_t)ring_list_getdouble(pList, 3);
	color.a = (uint8_t)ring_list_getdouble(pList, 4);

	int opacity = (int)RING_API_GETNUMBER(2);
	int result = osdialog_color_picker(&color, opacity);

	if (result)
	{
		ring_list_setdouble(pList, 1, color.r);
		ring_list_setdouble(pList, 2, color.g);
		ring_list_setdouble(pList, 3, color.b);
		ring_list_setdouble(pList, 4, color.a);
	}

	RING_API_RETNUMBER(result);
}

RING_FUNC(ring_get_dialog_info)
{
	RING_API_RETNUMBER(OSDIALOG_INFO);
}

RING_FUNC(ring_get_dialog_warning)
{
	RING_API_RETNUMBER(OSDIALOG_WARNING);
}

RING_FUNC(ring_get_dialog_error)
{
	RING_API_RETNUMBER(OSDIALOG_ERROR);
}

RING_FUNC(ring_get_dialog_ok)
{
	RING_API_RETNUMBER(OSDIALOG_OK);
}

RING_FUNC(ring_get_dialog_ok_cancel)
{
	RING_API_RETNUMBER(OSDIALOG_OK_CANCEL);
}

RING_FUNC(ring_get_dialog_yes_no)
{
	RING_API_RETNUMBER(OSDIALOG_YES_NO);
}

RING_FUNC(ring_get_dialog_open)
{
	RING_API_RETNUMBER(OSDIALOG_OPEN);
}

RING_FUNC(ring_get_dialog_open_dir)
{
	RING_API_RETNUMBER(OSDIALOG_OPEN_DIR);
}

RING_FUNC(ring_get_dialog_save)
{
	RING_API_RETNUMBER(OSDIALOG_SAVE);
}

RING_LIBINIT
{
	RING_API_REGISTER("dialog_message", ring_dialog_message);
	RING_API_REGISTER("dialog_prompt", ring_dialog_prompt);
	RING_API_REGISTER("dialog_file", ring_dialog_file);
	RING_API_REGISTER("dialog_color_picker", ring_dialog_color_picker);
	RING_API_REGISTER("get_dialog_info", ring_get_dialog_info);
	RING_API_REGISTER("get_dialog_warning", ring_get_dialog_warning);
	RING_API_REGISTER("get_dialog_error", ring_get_dialog_error);
	RING_API_REGISTER("get_dialog_ok", ring_get_dialog_ok);
	RING_API_REGISTER("get_dialog_ok_cancel", ring_get_dialog_ok_cancel);
	RING_API_REGISTER("get_dialog_yes_no", ring_get_dialog_yes_no);
	RING_API_REGISTER("get_dialog_open", ring_get_dialog_open);
	RING_API_REGISTER("get_dialog_open_dir", ring_get_dialog_open_dir);
	RING_API_REGISTER("get_dialog_save", ring_get_dialog_save);
}