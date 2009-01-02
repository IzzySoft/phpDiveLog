<?xml version="1.0" encoding="UTF-8"?>
<kml xmlns="http://earth.google.com/kml/2.2"
     xmlns:atom="http://www.w3.org/2005/Atom">
<Document>
  <name>{doc_name}</name>
  <open>{open}</open>
  <visibility>1</visibility>
  <atom:author>
    <atom:name>phpDiveLog</atom:name>
  </atom:author>
  <atom:link>http://projects.izzysoft.de/trac/phpdivelog</atom:link>
  <description><![CDATA[This list of divesites is provided by <a href="http://projects.izzysoft.de/trac/phpdivelog">phpDiveLog</a>.]]></description>
<!-- BEGIN linkblock -->
  <NetworkLink>
        <name>{link_name}</name>
        <visibility>1</visibility>
        <open>0</open>
        <description><![CDATA[{link_description}]]></description>
        <Link>
                <href>{link_url}</href>
		<refreshMode>onInterval</refreshMode>
                <refreshInterval>{link_refresh_interval}</refreshInterval>
		<viewRefreshMode>onRequest</viewRefreshMode>
                <viewRefreshTime>5</viewRefreshTime>
        </Link>
  </NetworkLink>
<!-- END linkblock -->
</Document>
</kml>
