# dsm_param
A drupal module which sets a message -- via the drupal_set_message function -- whenever 
any of a set of specified URL parameters is supplied by the client. For example, if you
configure the module to look for a URL parameter named "echo_me" and visit your site with
the following URL: http://www.example.com?echo_me=Test!, The page will have a message with
"Test!" on it.

To use, login to the site and visit the /admin/config/dsm_param/create page. Set up a new
parameter and visit your site with the parameter in the query string.

KNOW THE RISKS!
This is a a bit of a silly module which probably shouldn't be used on most sites. It
allows anyone linking to your site to set a message on the screen. While the module
should be secure, an unfriendly person could link to your site and have a nasty message
displayed. If you're confident that only trustworthy people will link to your site (this
is almost never the case), feel free to use this module.
