/**
 * @file lv_bar.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "CustomWidgets/threatzone/c_lv_threatzone.h"
#if LV_USE_BAR != 0

#include "LVGL/src/draw/lv_draw.h"
#include "LVGL/src/misc/lv_assert.h"
#include "LVGL/src/misc/lv_anim.h"
#include "LVGL/src/misc/lv_math.h"
#define _USE_MATH_DEFINES
#include <math.h>
/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &c_lv_threatzone_class

#ifndef M_PI
#define M_PI		3.14159
#endif

#ifndef M_PI_2
#define M_PI_2		1.57079
#endif

/** default indicator width for threatzone widget (if not set with setter func)*/
#define C_LV_THREATZONE_INDIC_WIDTH 30

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void c_lv_threatzone_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void c_lv_threatzone_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void c_lv_threatzone_event(const lv_obj_class_t * class_p, lv_event_t * e);
static void draw_indic(lv_event_t * e);
static int16_t c_lv_threatzone_getMaskAngle(int16_t centerAngle, int16_t offset);
static lv_draw_mask_line_side_t c_lv_threatzone_getLineMaskDirection(int16_t angle, bool maskSmaller);
static lv_point_t c_lv_threatzone_findIntersectionPoint(double angle, uint16_t rectangle_width, uint16_t rectangle_height, lv_point_t origin);
lv_area_t c_lv_threatzone_offset_area_to_origin(lv_point_t origin, lv_area_t area, int16_t offset);
void c_lv_threatzone_add_point_to_area(lv_area_t* area, lv_point_t point);

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t c_lv_threatzone_class = {
		.constructor_cb = c_lv_threatzone_constructor,
		.destructor_cb = c_lv_threatzone_destructor,
		.event_cb = c_lv_threatzone_event,
		.width_def = LV_DPI_DEF * 2,
		.height_def = LV_DPI_DEF / 10,
		.instance_size = sizeof(c_lv_threatzone_t),
		.base_class = &lv_obj_class
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * c_lv_threatzone_create(lv_obj_t * parent)
{
	LV_LOG_INFO("begin");
	lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, parent);
	lv_obj_class_init_obj(obj);
	return obj;
}

/*=====================
 * Setter functions
 *====================*/

void c_lv_threatzone_set_angle(lv_obj_t * obj, int32_t angle)
{
	LV_ASSERT_OBJ(obj, MY_CLASS);
	c_lv_threatzone_t * threatzone = (c_lv_threatzone_t *)obj;

	if(threatzone->center_angle == angle) return;

	threatzone->center_angle = angle;
	lv_obj_invalidate((lv_obj_t *)obj);
}

void c_lv_threatzone_set_draw_area(lv_obj_t * obj, lv_area_t draw_area)
{
	LV_ASSERT_OBJ(obj, MY_CLASS);
	c_lv_threatzone_t * threatzone = (c_lv_threatzone_t *)obj;

	if(memcmp(&threatzone->draw_area, &draw_area, sizeof(lv_area_t)))
	{
		memcpy(&threatzone->draw_area, &draw_area, sizeof(lv_area_t));
		lv_obj_invalidate((lv_obj_t *)obj);
	}
}

void c_lv_threatzone_set_origin(lv_obj_t * obj, lv_point_t origin)
{
	LV_ASSERT_OBJ(obj, MY_CLASS);
	c_lv_threatzone_t * threatzone = (c_lv_threatzone_t *)obj;

	if(memcmp(&threatzone->origin, &origin, sizeof(lv_point_t)))
	{
		memcpy(&threatzone->origin, &origin, sizeof(lv_point_t));
		lv_obj_invalidate((lv_obj_t *)obj);
	}
}

void c_lv_threatzone_set_size_offset(lv_obj_t * obj, int16_t offset)
{
	LV_ASSERT_OBJ(obj, MY_CLASS);
	c_lv_threatzone_t * threatzone = (c_lv_threatzone_t *)obj;

	if(threatzone->size_offset == offset) return;

	threatzone->size_offset = offset;
	lv_obj_invalidate((lv_obj_t *)obj);
}

void c_lv_threatzone_set_thickness(lv_obj_t * obj, int32_t thickness)
{
	LV_ASSERT_OBJ(obj, MY_CLASS);
	c_lv_threatzone_t * threatzone = (c_lv_threatzone_t *)obj;

	if(threatzone->indic_width == thickness) return;

	threatzone->indic_width = thickness;
	lv_obj_invalidate((lv_obj_t *)obj);
}

void c_lv_threatzone_set_confidence(lv_obj_t * obj, int32_t confidence)
{
	LV_ASSERT_OBJ(obj, MY_CLASS);
	c_lv_threatzone_t * threatzone = (c_lv_threatzone_t *)obj;

	if(threatzone->confidence == confidence) return;

	threatzone->confidence = confidence;
	lv_obj_invalidate((lv_obj_t *)obj);
}

/*=====================
 * Getter functions
 *====================*/

int32_t c_lv_threatzone_get_angle(const lv_obj_t * obj)
{
	LV_ASSERT_OBJ(obj, MY_CLASS);
	c_lv_threatzone_t * threatzone = (c_lv_threatzone_t *)obj;

	return threatzone->center_angle;
}

lv_area_t c_lv_threatzone_get_draw_area(const lv_obj_t * obj)
{
	LV_ASSERT_OBJ(obj, MY_CLASS);
	c_lv_threatzone_t * threatzone = (c_lv_threatzone_t *)obj;

	return threatzone->draw_area;
}

lv_point_t c_lv_threatzone_get_origin(const lv_obj_t * obj)
{
	LV_ASSERT_OBJ(obj, MY_CLASS);
	c_lv_threatzone_t * threatzone = (c_lv_threatzone_t *)obj;

	return threatzone->origin;
}

int32_t c_lv_threatzone_get_thickness(const lv_obj_t * obj)
{
	LV_ASSERT_OBJ(obj, MY_CLASS);
	c_lv_threatzone_t * threatzone = (c_lv_threatzone_t *)obj;

	return threatzone->indic_width;
}

int32_t c_lv_threatzone_get_confidence(const lv_obj_t * obj)
{
	LV_ASSERT_OBJ(obj, MY_CLASS);
	c_lv_threatzone_t * threatzone = (c_lv_threatzone_t *)obj;

	return threatzone->confidence;
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

static void c_lv_threatzone_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
	LV_UNUSED(class_p);
	LV_TRACE_OBJ_CREATE("begin");

	c_lv_threatzone_t * threatzone = (c_lv_threatzone_t *)obj;
	threatzone->center_angle = 0;
	threatzone->confidence = 100;
	threatzone->draw_area.x1 = 0;
	threatzone->draw_area.x2 = 0;
	threatzone->draw_area.y1 = 0;
	threatzone->draw_area.y2 = 0;
	threatzone->indic_width = C_LV_THREATZONE_INDIC_WIDTH;
	threatzone->origin.x = 0;
	threatzone->origin.y = 0;

	lv_obj_clear_flag(obj, LV_OBJ_FLAG_CHECKABLE);
	lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	LV_TRACE_OBJ_CREATE("finished");
}

static void c_lv_threatzone_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
	LV_UNUSED(class_p);
	LV_UNUSED(obj);
}

static void draw_indic(lv_event_t * e)
{
	/*--- Get object handle and driver ctx ---*/
	lv_obj_t * obj = lv_event_get_target(e);
	c_lv_threatzone_t * threat = (c_lv_threatzone_t *) obj;
	lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);
    lv_obj_t* parent = lv_obj_get_parent(obj);

	/*--- We will need two line masks that will mask the entire widget ---*/
	lv_draw_mask_line_param_t maskStart;
	lv_draw_mask_line_param_t maskEnd;

	/*--- Calculate basic info about origin, angles etc) ---*/
	int16_t startAngle = c_lv_threatzone_getMaskAngle(threat->center_angle, -(threat->confidence / 2));
	int16_t endAngle = c_lv_threatzone_getMaskAngle(threat->center_angle, (threat->confidence / 2));

	/*--- Calculate the start and end line masks ---*/
    lv_draw_mask_line_side_t startMaskDirection = c_lv_threatzone_getLineMaskDirection(startAngle, true);
	lv_draw_mask_line_side_t endMaskDirection = c_lv_threatzone_getLineMaskDirection(endAngle, false);

    lv_point_t origin;

    lv_area_t parent_content_coords;
    lv_obj_get_content_coords(parent, &parent_content_coords);
    origin.x = parent_content_coords.x1 + threat->origin.x;
    origin.y = parent_content_coords.y1 + threat->origin.y;

    lv_draw_mask_line_angle_init(&maskStart, origin.x, origin.y, startAngle, startMaskDirection);
    lv_draw_mask_line_angle_init(&maskEnd, origin.x, origin.y, endAngle, endMaskDirection);

    lv_coord_t mask_start_id;
    lv_coord_t mask_end_id;

    if (threat->confidence <= 180)
    {
        mask_start_id = lv_draw_mask_add(&maskStart, NULL);
        mask_end_id = lv_draw_mask_add(&maskEnd, NULL);
    }

	lv_draw_rect_dsc_t draw_outer_dsc;
	lv_draw_rect_dsc_init(&draw_outer_dsc);
	lv_obj_init_draw_rect_dsc(obj, LV_PART_INDICATOR, &draw_outer_dsc);
    draw_outer_dsc.border_opa = LV_OPA_TRANSP;
    draw_outer_dsc.border_width = 0;
    draw_outer_dsc.shadow_opa = LV_OPA_TRANSP;
    draw_outer_dsc.bg_opa = LV_OPA_COVER;
    draw_outer_dsc.bg_color = lv_obj_get_style_bg_color(obj, LV_PART_INDICATOR);
    draw_outer_dsc.bg_img_opa = LV_OPA_TRANSP;
    draw_outer_dsc.shadow_opa = LV_OPA_TRANSP;
    draw_outer_dsc.blend_mode = LV_BLEND_MODE_NORMAL;

    lv_area_t outerArea = parent_content_coords;
    outerArea.x1 += threat->size_offset;
    outerArea.x2 -= threat->size_offset;
    outerArea.y1 += threat->size_offset;
    outerArea.y2 -= threat->size_offset;

	lv_draw_rect(draw_ctx, &draw_outer_dsc, &outerArea);

    lv_draw_rect_dsc_t draw_innerrect_dsc;
    lv_draw_rect_dsc_init(&draw_innerrect_dsc);
    lv_obj_init_draw_rect_dsc(obj, LV_PART_INDICATOR, &draw_innerrect_dsc);
    draw_innerrect_dsc.shadow_opa = LV_OPA_TRANSP;
    draw_innerrect_dsc.border_opa = LV_OPA_TRANSP;
    draw_innerrect_dsc.border_width = 0;
    draw_innerrect_dsc.bg_opa = LV_OPA_80;
    draw_innerrect_dsc.bg_color = lv_obj_get_style_bg_color(obj, LV_PART_INDICATOR);
    draw_innerrect_dsc.bg_img_opa = LV_OPA_TRANSP;
    draw_innerrect_dsc.shadow_opa = LV_OPA_TRANSP;
    draw_innerrect_dsc.blend_mode = LV_BLEND_MODE_SUBTRACTIVE;

    //lv_area_t innerArea = c_lv_threatzone_offset_area_to_origin(origin, obj->coords, C_LV_THREATZONE_INDIC_WIDTH);
    lv_area_t innerArea = parent_content_coords;
    innerArea.x1 += C_LV_THREATZONE_INDIC_WIDTH + threat->size_offset;
    innerArea.x2 -= (C_LV_THREATZONE_INDIC_WIDTH + threat->size_offset);
    innerArea.y1 += C_LV_THREATZONE_INDIC_WIDTH + threat->size_offset;
    innerArea.y2 -= (C_LV_THREATZONE_INDIC_WIDTH + threat->size_offset);
    lv_draw_rect(draw_ctx, &draw_innerrect_dsc, &innerArea);


    if (threat->confidence <= 180)
    {
        lv_draw_mask_free_param(&maskStart);
        lv_draw_mask_remove_id(mask_start_id);
        lv_draw_mask_free_param(&maskEnd);
        lv_draw_mask_remove_id(mask_end_id);
    }

	lv_event_send(obj, LV_EVENT_DRAW_PART_END, &draw_innerrect_dsc);
}

static void c_lv_threatzone_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
	LV_UNUSED(class_p);

	lv_res_t res;

	/*Call the ancestor's event handler*/
	res = lv_obj_event_base(MY_CLASS, e);
	if(res != LV_RES_OK) return;

	lv_event_code_t code = lv_event_get_code(e);

	if(code == LV_EVENT_REFR_EXT_DRAW_SIZE)
	{

	}
	else if(code == LV_EVENT_DRAW_MAIN)
	{
		draw_indic(e);
	}
}

static int16_t c_lv_threatzone_getMaskAngle(int16_t centerAngle, int16_t offset)
{
	int16_t maskAngle = centerAngle + offset - 90;

	if(maskAngle >= 360)
	{
		maskAngle %= 360;
	}
	else if(maskAngle < 0)
	{
		while(maskAngle < 0)
		{
			maskAngle += 360;
		}
	}

	return maskAngle;
}

static lv_draw_mask_line_side_t c_lv_threatzone_getLineMaskDirection(int16_t angle, bool maskSmaller)
{

	if(angle > 0 && angle <= 90)
	{
		return !maskSmaller ? LV_DRAW_MASK_LINE_SIDE_RIGHT : LV_DRAW_MASK_LINE_SIDE_LEFT;
	}
	else if(angle > 90 && angle <= 180)
	{
		return !maskSmaller ? LV_DRAW_MASK_LINE_SIDE_BOTTOM : LV_DRAW_MASK_LINE_SIDE_TOP;
	}
	else if(angle > 180 && angle <= 270)
	{
		return !maskSmaller ? LV_DRAW_MASK_LINE_SIDE_LEFT : LV_DRAW_MASK_LINE_SIDE_RIGHT;
	}
	else
	{
		return !maskSmaller ? LV_DRAW_MASK_LINE_SIDE_TOP : LV_DRAW_MASK_LINE_SIDE_BOTTOM;
	}
}

void c_lv_threatzone_calculate_area(c_lv_threatzone_t* threat)
{
	lv_obj_t* obj = (lv_obj_t*) threat;
	lv_obj_t* parent = lv_obj_get_parent(obj);

	uint16_t width = lv_obj_get_content_width(parent) - 2 * threat->size_offset;
	uint16_t height = lv_obj_get_content_height(parent) - 2 * threat->size_offset;

    if (threat->confidence > 180)
    {
        lv_obj_set_width(obj, width);
        lv_obj_set_height(obj, height);
        threat->draw_area.x1 = threat->size_offset;
        threat->draw_area.x2 = width;
        threat->draw_area.y1 = threat->size_offset;
        threat->draw_area.y2 = height;
        return;
    }

	double startRadians = (((double) threat->center_angle - ((double) threat->confidence / (double) 2)) * (double) 3.14159) / (double) 180;
	double endRadians = (((double) threat->center_angle + ((double) threat->confidence / (double) 2)) * (double) 3.14159) / (double) 180;
    lv_point_t relativeOrigin = threat->origin;
    relativeOrigin.x -= threat->size_offset;
    relativeOrigin.y -= threat->size_offset;
	lv_point_t startIntersect = c_lv_threatzone_findIntersectionPoint(startRadians, width, height, relativeOrigin);
	lv_point_t endIntersect = c_lv_threatzone_findIntersectionPoint(endRadians, width, height, relativeOrigin);

    startIntersect.x += threat->size_offset;
    startIntersect.y += threat->size_offset;
    endIntersect.x += threat->size_offset;
    endIntersect.y += threat->size_offset;

	//Start at the origin
	threat->draw_area.x1 = threat->draw_area.x2 = threat->origin.x;
	threat->draw_area.y1 = threat->draw_area.y2 = threat->origin.y;

    //Add all the point to the area to finc the area whioch encloses all the points
    c_lv_threatzone_add_point_to_area(&threat->draw_area, threat->origin);
    c_lv_threatzone_add_point_to_area(&threat->draw_area, startIntersect);
    c_lv_threatzone_add_point_to_area(&threat->draw_area, endIntersect);

    //Add the corners as required
    static lv_point_t corners[4];
    static double radiansToCorners[4];

    if (radiansToCorners[0] == 0)
    {

        radiansToCorners[0] = atan(((double)width - (double)relativeOrigin.x) / ((double)height - (double)relativeOrigin.y));      //Top right
        radiansToCorners[1] = M_PI_2 + atan(((double)relativeOrigin.y) / ((double)width - (double)relativeOrigin.x));            //Bot right
        radiansToCorners[2] = M_PI + atan(((double)relativeOrigin.x) / ((double)height - (double)relativeOrigin.y));              //Bot left
        radiansToCorners[3] = 3 * M_PI_2 + atan(((double)height - (double)relativeOrigin.y) / ((double)relativeOrigin.x));       //Top left

    }

    //Top right
    corners[0].x = width + +threat->size_offset;
    corners[0].y = threat->size_offset;

    //Bot right
    corners[1].x = width + threat->size_offset;
    corners[1].y = height + threat->size_offset;

    //Bot left
    corners[2].x = threat->size_offset;
    corners[2].y = height + +threat->size_offset;

    //Top left
    corners[3].x = threat->size_offset;
    corners[3].y = height + threat->size_offset;

    //Ensure both start and end are positive
    while (startRadians < 0)
    {
        startRadians += 2 * M_PI;
    }

    while (endRadians < 0)
    {
        endRadians += 2 * M_PI;
    }

    if (endRadians < startRadians)
    {
        //We go past 0
        for (uint8_t i = 0; i < 4; i++)
        {
            if (startRadians < radiansToCorners[i])
            {
                c_lv_threatzone_add_point_to_area(&threat->draw_area, corners[i]);
            }
            else if (endRadians > radiansToCorners[i])
            {
                c_lv_threatzone_add_point_to_area(&threat->draw_area, corners[i]);
            }
        }
    }
    else
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            if ((startRadians < radiansToCorners[i] && radiansToCorners[i] < endRadians) || threat->confidence == 0)
            {
                c_lv_threatzone_add_point_to_area(&threat->draw_area, corners[i]);
            }
        }
    }
    

    //Save the new width and height
    lv_obj_set_width(obj, threat->draw_area.x2 - threat->draw_area.x1);
    lv_obj_set_height(obj, threat->draw_area.y2 - threat->draw_area.y1);
}

void c_lv_threatzone_calculate_position(c_lv_threatzone_t* threat)
{
    lv_obj_t* obj = (lv_obj_t*)threat;

    if (lv_obj_get_x(obj) != threat->draw_area.x1)
    {
        lv_obj_set_x(obj, threat->draw_area.x1);
    }

    if (lv_obj_get_y(obj) != threat->draw_area.y1)
    {
        lv_obj_set_y(obj, threat->draw_area.y1);
    }
}

/**
 * @brief  	Calculates the point at which a line will intersect a rectangle
 * @note	This function converts the y-coordinate to cartetian (positive is up)
 * @param	Angle in radians from the positive y-axis (measured clockwise)
 * @param	Width of the rectangle
 * @param	Height of the rectangle
 * @param	Coordinates of the lines origin (relative to the start of the rectangle)
 * @retval 	Coordinate of the intersection
 */
static lv_point_t c_lv_threatzone_findIntersectionPoint(double angle, uint16_t rectangle_width, uint16_t rectangle_height, lv_point_t origin)
{
	lv_point_t intersect = origin;

    //Enusre the angle is positive
    while (angle < 0)
    {
        angle += 2 * M_PI;
    }

	//Ensure the point is within the rectangle
	if(origin.x > rectangle_width || origin.y > rectangle_height)
	{
		return origin;
	}

	//Ensure the origin is not on one of the edges
	if(origin.x == 0 || origin.x == rectangle_width || origin.y == 0 || origin.y == rectangle_height)
	{
		return origin;
	}

	//Check if the line is exactly horizontal or vertical
	if(angle == 0)	//Pointing up
	{
		intersect.y = 0;
		return intersect;
	}
	else if(angle == M_PI_2) //Pointing Right
	{
		intersect.x = rectangle_width;
		return intersect;
	}
	else if(angle == M_PI) //Pointing Down
	{
		intersect.y = rectangle_height;
		return intersect;
	}
	else if(angle == 3 * M_PI_2) //Pointing Left
	{
		intersect.x = 0;
		return intersect;
	}

	//Flip the y-coordinate
	intersect.y = rectangle_height - intersect.y;

	//Get definition of the line (y = mx + c)
	double c = intersect.y;
    double a = M_PI_2 - angle;
	double m = tan(a);

	//Determine which edge we are intersecting by calculating the angle to each corner
	static double radiansToCorners[4];

    if (radiansToCorners[0] == 0)
    {
        radiansToCorners[0] = atan(((double)rectangle_width - (double)origin.x) / ((double)rectangle_height - (double)origin.y));  //Top right
        radiansToCorners[1] = M_PI_2 + atan(((double)origin.y) / ((double)rectangle_width - (double)origin.x));            //Bot right
        radiansToCorners[2] = M_PI + atan(((double)origin.x) / ((double)rectangle_height - (double)origin.y));              //Bot left
        radiansToCorners[3] = 3 * M_PI_2 + atan(((double)rectangle_height - (double)origin.y) / ((double)origin.x));       //Top left
    }

	//Top edge
	if (angle > radiansToCorners[3] || angle < radiansToCorners[0])
	{
        double xans = round((rectangle_height - c) / m);
		intersect.x = origin.x + xans;
		intersect.y = 0;
		return intersect;
	}
	//Right edge
	else if(angle > radiansToCorners[0] && angle < radiansToCorners[1])
	{
		intersect.y = round(m * (rectangle_width - intersect.x) + c);
		intersect.x = rectangle_width;
		intersect.y = rectangle_height - intersect.y; //Flip y around again
		return intersect;
	}
	//Bottom edge
	else if(angle > radiansToCorners[1] && angle < radiansToCorners[2])
	{
		double xans = - c / m;
		intersect.x = intersect.x + round(xans);
		intersect.y = rectangle_height;
		return intersect;
	}
	//Left edge
	else if(angle > radiansToCorners[2] && angle < radiansToCorners[3])
	{
        intersect.y = round(m * (- intersect.x) + c);
        intersect.x = 0;
        intersect.y = rectangle_height - intersect.y; //Flip y around again
		return intersect;
	}

	return intersect;
}

lv_area_t c_lv_threatzone_offset_area_to_origin(lv_point_t origin, lv_area_t area, int16_t offset)
{
    lv_area_t newArea = area;

    //First case is origin lies on the bottom edge
    if (origin.x <= area.x1 + 2 && origin.x >= area.x1 - 2)
    {
        newArea.x2 -= offset;
    }

    if (origin.x <= area.x2 + 2 && origin.x >= area.x2 - 2)
    {
        newArea.x1 += offset;
    }

    if (origin.y <= area.y1 + 2 && origin.y >= area.y1 - 2)
    {
        newArea.y2 -= offset;
    }

    if (origin.y <= area.y2 + 2 && origin.y >= area.y2 - 2)
    {
        newArea.y1 += offset;
    }

    return newArea;
}

/**
 * @brief  	Enlarges an area until it enclosures the point
 * @retval 	None
 */
void c_lv_threatzone_add_point_to_area(lv_area_t* area, lv_point_t point)
{
    if (point.x < area->x1)
    {
        area->x1 = point.x;
    }

    if (point.y < area->y1)
    {
        area->y1 = point.y;
    }

    if (point.x > area->x2)
    {
        area->x2 = point.x;
    }

    if (point.y > area->y2)
    {
        area->y2 = point.y;
    }
}

#endif

