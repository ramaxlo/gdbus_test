
#include "test-server-generated.h"

/* ---------------------------------------------------------------------------------------------------- */
static TestServer *server = NULL;
static gboolean online = FALSE;

static gboolean on_handle_hello(TestServer *server, GDBusMethodInvocation *invocation,
		const char *input, gpointer data)
{
	GString *string = NULL;

	string = g_string_new("Hello");
	g_string_append_printf(string, " %s", input);

	test_server_complete_hello(server, invocation, string->str);
	g_string_free(string, TRUE);

	return TRUE;
}

static void on_bus_acquired (GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
	GError *error = NULL;

	server = test_server_skeleton_new();
	g_assert(server);

	g_signal_connect(server, "handle-hello", G_CALLBACK(on_handle_hello), NULL);
	test_server_set_name(server, "TestServer");

	g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(server),
					connection,
					"/com/umbocv/Test/Server",
					&error);
	if (error)
	{
		g_print("Error: %s\n", error->message);
		g_error_free(error);

		return;
	}

	g_print ("Acquired a message bus connection\n");
}

static void on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
	g_print ("Acquired the name %s\n", name);
}

static void on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
	g_print ("Lost the name %s\n", name);
}

gboolean on_timeout(gpointer data)
{
	gchar *status;

	if (!server)
		return G_SOURCE_CONTINUE;

	if (online)
	{
		status = "offline";
		online = FALSE;
	}
	else
	{
		status = "online";
		online = TRUE;
	}

	test_server_emit_status_changed(server, status);

	return G_SOURCE_CONTINUE;
}

gint main (gint argc, gchar *argv[])
{
	GMainLoop *loop;
	guint id;

	loop = g_main_loop_new (NULL, FALSE);

	id = g_bus_own_name (G_BUS_TYPE_SESSION,
			"com.umbocv.Test",
			G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT |
			G_BUS_NAME_OWNER_FLAGS_REPLACE,
			on_bus_acquired,
			on_name_acquired,
			on_name_lost,
			loop,
			NULL);

	g_timeout_add(5000, on_timeout, NULL);

	g_main_loop_run (loop);

	g_bus_unown_name (id);
	g_main_loop_unref (loop);

	return 0;
}
