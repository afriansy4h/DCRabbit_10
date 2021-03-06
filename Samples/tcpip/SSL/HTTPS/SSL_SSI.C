/*
   Copyright (c) 2015, Digi International Inc.

   Permission to use, copy, modify, and/or distribute this software for any
   purpose with or without fee is hereby granted, provided that the above
   copyright notice and this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
/*******************************************************************************
        ssl_ssi.c

        A basic contoller. This creates four 'devices' (lights), and four
        buttons to toggle them. Users can browse to the device, and change
        the status of the lights.

		  This program has been modified to use SSL via HTTPS. See the
        SSL comments below for more information.

        ***NOTE*** This sample will NOT compile without first creating a
        certificate with the name and path specified in the #ximport line
        ("#ximport 'cert\mycerts.pem' server_pub_cert") below. See the SSL
        Walkthrough, Section 4.1 in the SSL User's Manual for information
        on creating certificates for SSL programs.

        As of Dynamic C 10.66, it is recommended to use .pem format
        certificates rather than the deprecated .dcc format.  Both formats
        are generated by the certificate utility, however .pem is faster
        at run-time, and is a non-proprietary format.

        Note that this sample uses both HTTP and HTTPS. This means that
        unprotected resources (see zserver.lib and flashspec) can be
        accessed via both HTTP and HTTPS.
*******************************************************************************/
#class auto

/***********************************
 * Configuration                   *
 * -------------                   *
 * All fields in this section must *
 * be altered to match your local  *
 * network settings.               *
 ***********************************/

/*
 * NETWORK CONFIGURATION
 * Please see the function help (Ctrl-H) on TCPCONFIG for instructions on
 * compile-time network configuration.
 */
#define TCPCONFIG 1


/*
 * TCP/IP modification - reduce TCP socket buffer
 * size, to allow more connections. This can be increased,
 * with increased performance, if the number of sockets
 * are reduced.  Note that this buffer size is split in
 * two for TCP sockets--1024 bytes for send and 1024 bytes
 * for receive.
 */
#define TCP_BUF_SIZE 2048

/*
 * Web server configuration
 */

/*
 * only one socket and server are needed for a reserved port,
 * but to use HTTP and HTTPS on the same device, 2 servers
 * are needed, one for HTTP and one for HTTPS. To use only
 * HTTPS, change HTTP_MAXSERVERS to match HTTP_SSL_SOCKETS.
 */
#define HTTP_MAXSERVERS 2
#define MAX_TCP_SOCKET_BUFFERS 2

// This macro determines the number of HTTP servers that will use
// SSL (HTTPS servers). In this case, we have 2 total servers, and
// this defines one of them to be HTTPS
#define HTTP_SSL_SOCKETS 1


// SSL Stuff
// This macro tells the HTTP library to use SSL
#define USE_HTTP_SSL

// Import the certificate and corresponding private key.
// The Dynamic C Certificate Generation Utility will create these files
// (if XXX is the basic certificate name, then the files will be called
// XXXs.pem and XXXkey.pem).
#ximport "cert\mycerts.pem" server_pub_cert
#ximport "cert\mycertkey.pem" server_priv_key

// If defined, remove code which supports legacy .dcc format certificates.
#define SSL_DISABLE_LEGACY_DCC

// If defined, remove code which supports storing certificates in the UserBlock.
#define SSL_DISABLE_USERBLOCK

/********************************
 * End of configuration section *
 ********************************/

/*
 *  REDIRECTTO is used by each ledxtoggle cgi's to tell the
 *  browser which page to hit next.
 *
 */

#define REDIRECTTO 		"/"

#memmap xmem
#use "dcrtcp.lib"
#use "http.lib"

/*
 *  Notice that we have ximported in the source code for
 *  this program.  This allows us to <!--#include file="ssi.c"-->
 *  in the pages/showsrc.shtml.
 *
 */
#ximport "pages/ledon.gif"      ledon_gif
#ximport "pages/ssi.shtml"      index_html
#ximport "pages/rabbit1.gif"    rabbit1_gif
#ximport "pages/ledoff.gif"     ledoff_gif
#ximport "pages/button.gif"     button_gif
#ximport "pages/showsrc.shtml"  showsrc_shtml
#ximport "ssl_ssi.c"            ssi_c

/*
 *  In this case the .html is not the first type in the
 *  type table.  This causes the default (no extension)
 *  to assume the shtml_handler.
 *
 */
/* the default mime type for files without an extension must be first */
SSPEC_MIMETABLE_START
   SSPEC_MIME_FUNC(".shtml", "text/html", shtml_handler),
	SSPEC_MIME(".html", "text/html"),
	SSPEC_MIME(".gif", "image/gif")
SSPEC_MIMETABLE_END

/*
 *  Each ledx contains a text string that is either
 *  "ledon.gif" or "ledoff.gif"  This string is toggled
 *  each time the ledxtoggle.cgi is requested from the
 *  browser.
 *
 */

char led1[15];
char led2[15];
char led3[15];
char led4[15];

/*
 *  Instead of sending other text back from the cgi's
 *  we have decided to redirect them to the original page.
 *  the cgi_redirectto forms a header which will redirect
 *  the browser back to the main page.
 *
 */

int led1toggle(HttpState* state)
{
   if (strcmp(led1,"ledon.gif")==0)
      strcpy(led1,"ledoff.gif");
   else
      strcpy(led1,"ledon.gif");

   cgi_redirectto(state,REDIRECTTO);
   return 0;
}

int led2toggle(HttpState* state)
{
   if (strcmp(led2,"ledon.gif")==0)
      strcpy(led2,"ledoff.gif");
   else
      strcpy(led2,"ledon.gif");

   cgi_redirectto(state,REDIRECTTO);
   return 0;
}

int led3toggle(HttpState* state)
{
   if (strcmp(led3,"ledon.gif")==0)
      strcpy(led3,"ledoff.gif");
   else
      strcpy(led3,"ledon.gif");

   cgi_redirectto(state,REDIRECTTO);
   return 0;
}

int led4toggle(HttpState* state)
{
   if (strcmp(led4,"ledon.gif")==0)
      strcpy(led4,"ledoff.gif");
   else
      strcpy(led4,"ledon.gif");

   cgi_redirectto(state,REDIRECTTO);
   return 0;
}

SSPEC_RESOURCETABLE_START
	SSPEC_RESOURCE_XMEMFILE("/",              index_html),
	SSPEC_RESOURCE_XMEMFILE("/showsrc.shtml", showsrc_shtml),
	SSPEC_RESOURCE_XMEMFILE("/ledon.gif",     ledon_gif),
	SSPEC_RESOURCE_XMEMFILE("/rabbit1.gif",   rabbit1_gif),	
	SSPEC_RESOURCE_XMEMFILE("/ledoff.gif",    ledoff_gif),
	SSPEC_RESOURCE_XMEMFILE("/button.gif",    button_gif),

	SSPEC_RESOURCE_XMEMFILE("/ssi.c",         ssi_c),

	SSPEC_RESOURCE_ROOTVAR("led1",            led1, PTR16, "%s"),
	SSPEC_RESOURCE_ROOTVAR("led2",            led2, PTR16, "%s"),
	SSPEC_RESOURCE_ROOTVAR("led3",            led3, PTR16, "%s"),
	SSPEC_RESOURCE_ROOTVAR("led4",            led4, PTR16, "%s"),

	SSPEC_RESOURCE_FUNCTION("/led1tog.cgi",   led1toggle),
	SSPEC_RESOURCE_FUNCTION("/led2tog.cgi",   led2toggle),
	SSPEC_RESOURCE_FUNCTION("/led3tog.cgi",   led3toggle),
	SSPEC_RESOURCE_FUNCTION("/led4tog.cgi",   led4toggle)
SSPEC_RESOURCETABLE_END

void main()
{
	// Can't store this on the stack (auto) since the HTTP server library stores
	// a reference to it for use later.
	static far SSL_Cert_t my_cert;

   strcpy(led1,"ledon.gif");
   strcpy(led2,"ledon.gif");
   strcpy(led3,"ledoff.gif");
   strcpy(led4,"ledon.gif");

	// Start network and wait for interface to come up (or error exit).
	sock_init_or_exit(1);
   http_init();

	_f_memset(&my_cert, 0, sizeof(my_cert));
	// When using HTTPS (i.e. HTTP over SSL or TLS), the certificates need
	// to be parsed and registered with the library.  For use with a
	// server, we need to know our own private key.
	if (SSL_new_cert(&my_cert, server_pub_cert, SSL_DCERT_XIM, 0) ||
	    SSL_set_private_key(&my_cert, server_priv_key, SSL_DCERT_XIM))
		exit(7);

	// Register certificate with HTTPS server.
	https_set_cert(&my_cert);

	/*
	 *  tcp_reserveport causes the web server to ignore requests when there
	 *  isn't an available socket (HTTP_MAXSERVERS are all serving index_html
	 *  or rabbit1.gif).  This saves some memory, but can cause the client
	 *  delays when retrieving pages.
	 */

   tcp_reserveport(HTTP_PORT);

   // Also reserve the HTTPS port
   tcp_reserveport(HTTPS_PORT);

	/*
	 *  http_handler needs to be called to handle the active http servers.
	 */

   while (1) {
      http_handler();
   }
}

