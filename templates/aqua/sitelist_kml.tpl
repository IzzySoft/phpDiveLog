<?xml version="1.0" encoding="UTF-8"?>
<kml xmlns="http://earth.google.com/kml/2.2">
<Document>
	<name>{docname}</name>
	<Style id="s_fins">
		<IconStyle>
			<scale>1.2</scale>
			<Icon>
				<href>{site_icon}</href>
			</Icon>
		</IconStyle>
		<ListStyle>
		</ListStyle>
	</Style>
	<StyleMap id="sm_fins">
		<Pair>
			<key>normal</key>
			<styleUrl>#s_fins</styleUrl>
		</Pair>
		<Pair>
			<key>highlight</key>
			<styleUrl>#s_fins</styleUrl>
		</Pair>
	</StyleMap>
	<Folder>
		<name>{foldername}</name>
		<open>1</open>
<!-- BEGIN itemblock -->
		<Placemark>
			<name>{placemarkname}</name>
			<description><![CDATA[{description}
                        ]]></description>
			<LookAt>
				<longitude>{longitude}</longitude>
				<latitude>{latitude}</latitude>
				<altitude>{altitude}</altitude>
				<range>{viewingdistance}</range>
				<tilt>0</tilt>
				<heading>0</heading>
			</LookAt>
			<styleUrl>s_fins</styleUrl>
			<Point>
				<coordinates>{longitude},{latitude},{altitude}</coordinates>
			</Point>
		</Placemark>
<!-- END itemblock -->
	</Folder>
</Document>
</kml>
