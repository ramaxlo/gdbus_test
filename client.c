
#include "test-server-generated.h"

/* ---------------------------------------------------------------------------------------------------- */

static gboolean on_status_changed(TestServer *server, const gchar *status, gpointer data)
{
	g_print("Status: %s\n", status);

	return TRUE;
}

gint main (gint argc, gchar *argv[])
{
	GError *error;
	gchar *name_owner;
	TestServer *server;
	gboolean rc;
	gchar *output;
	GMainLoop *loop;

	loop = g_main_loop_new(NULL, FALSE);

	error = NULL;
	server = test_server_proxy_new_for_bus_sync (G_BUS_TYPE_SESSION,
						G_DBUS_PROXY_FLAGS_NONE,
						"com.umbocv.Test",
						"/com/umbocv/Test/Server",
						NULL, /* GCancellable */
						&error);
	if (server == NULL)
	{
		g_printerr ("Error getting object manager client: %s", error->message);
		g_error_free (error);
		goto out;
	}

	name_owner = g_dbus_proxy_get_name_owner (G_DBUS_PROXY (server));
	g_print ("name-owner: %s\n", name_owner);
	g_free (name_owner);

	rc = test_server_call_hello_sync(server, "abc", &output, NULL, &error);
	if (!rc)
	{
		g_printerr("Error calling hello: %s\n", error->message);
		g_error_free(error);
		goto out;
	}

	g_print("Output: %s\n", output);
	g_free(output);

	g_signal_connect(server, "status-changed", G_CALLBACK(on_status_changed), NULL);

	g_main_loop_run(loop);

out:
	if (server != NULL)
		g_object_unref (server);
	if (loop)
		g_main_loop_unref(loop);

	return 0;
}
