#include "openbox/actions.h"
#include "openbox/screen.h"
#include "openbox/client.h"
#include <glib.h>

typedef struct {
    gboolean linear;
    gboolean wrap;
    ObDirection dir;
    gboolean send;
    gboolean follow;
} Options;

static gpointer setup_func(ObParseInst *i, xmlDocPtr doc, xmlNodePtr node);
static void     free_func(gpointer options);
static gboolean run_func(ObActionsData *data, gpointer options);

void action_directionaldesktop_startup()
{
    actions_register("DirectionalDesktop",
                     setup_func,
                     free_func,
                     run_func,
                     NULL, NULL);
}

static gpointer setup_func(ObParseInst *i, xmlDocPtr doc, xmlNodePtr node)
{
    xmlNodePtr n;
    Options *o;

    o = g_new0(Options, 1);
    o->wrap = TRUE;
    o->dir = OB_DIRECTION_EAST;
    o->follow = TRUE;

    if ((n = parse_find_node("wrap", node)))
        o->wrap = parse_bool(doc, n);
    if ((n = parse_find_node("direction", node))) {
        gchar *s = parse_string(doc, n);
        if (!g_ascii_strcasecmp(s, "next")) {
            o->linear = TRUE;
            o->dir = OB_DIRECTION_EAST;
        }
        else if (!g_ascii_strcasecmp(s, "previous")) {
            o->linear = TRUE;
            o->dir = OB_DIRECTION_WEST;
        }
        else if (!g_ascii_strcasecmp(s, "north") ||
                 !g_ascii_strcasecmp(s, "up"))
            o->dir = OB_DIRECTION_NORTH;
        else if (!g_ascii_strcasecmp(s, "south") ||
                 !g_ascii_strcasecmp(s, "down"))
            o->dir = OB_DIRECTION_SOUTH;
        else if (!g_ascii_strcasecmp(s, "west") ||
                 !g_ascii_strcasecmp(s, "left"))
            o->dir = OB_DIRECTION_WEST;
        else if (!g_ascii_strcasecmp(s, "east") ||
                 !g_ascii_strcasecmp(s, "right"))
            o->dir = OB_DIRECTION_EAST;
        g_free(s);
    }
    if ((n = parse_find_node("send", node)))
        o->send = parse_bool(doc, n);
    if ((n = parse_find_node("follow", node)))
        o->follow = parse_bool(doc, n);

    return o;
}

static void free_func(gpointer options)
{
    Options *o = options;

    g_free(o);
}

/* Always return FALSE because its not interactive */
static gboolean run_func(ObActionsData *data, gpointer options)
{
    Options *o = options;
    guint d;

    d = screen_cycle_desktop(o->dir,
                             o->wrap,
                             o->linear,
                             FALSE, TRUE, FALSE);
    if (d < screen_num_desktops && d != screen_desktop) {
        gboolean go = !o->send;
        if (o->send) {
            if (data->client && client_normal(data->client)) {
                client_set_desktop(data->client, d, o->follow, FALSE);
                go = TRUE;
            }
        }
        if (go)
            screen_set_desktop(d, TRUE);
    }

    return FALSE;
}
