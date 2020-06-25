/* Magic Mirror Config Sample
 *
 * By Michael Teeuw http://michaelteeuw.nl
 * MIT Licensed.
 *
 * For more information on how you can configure this file
 * See https://github.com/MichMich/MagicMirror#configuration
 *
 */

var config = {
	address: "0.0.0.0", // Address to listen on, can be:
	                      // - "localhost", "127.0.0.1", "::1" to listen on loopback interface
	                      // - another specific IPv4/6 to listen on a specific interface
	                      // - "0.0.0.0", "::" to listen on any interface
	                      // Default, when address config is left out or empty, is "localhost"
	port: 8080,
	ipWhitelist: ["127.0.0.1", "::ffff:127.0.0.1", "::1","192.168.10.0/24"], // Set [] to allow all IP addresses
	                                                       // or add a specific IPv4 of 192.168.1.5 :
	                                                       // ["127.0.0.1", "::ffff:127.0.0.1", "::1", "::ffff:192.168.1.5"],
	                                                       // or IPv4 range of 192.168.3.0 --> 192.168.3.15 use CIDR format :
	                                                       // ["127.0.0.1", "::ffff:127.0.0.1", "::1", "::ffff:192.168.3.0/28"],

	useHttps: false, 		// Support HTTPS or not, default "false" will use HTTP
	httpsPrivateKey: "", 	// HTTPS private key path, only require when useHttps is true
	httpsCertificate: "", 	// HTTPS Certificate path, only require when useHttps is true

	language: "en",
	timeFormat: 24,
	units: "metric",
	// serverOnly:  true/false/"local" ,
			     // local for armv6l processors, default
			     //   starts serveronly and then starts chrome browser
			     // false, default for all  NON-armv6l devices
			     // true, force serveronly mode, because you want to.. no UI on this device

	modules: [

		{
			module: "alert",
		},
		//{
		//	module: "updatenotification",
		//	position: "top_bar"
		//},
		{
			module: "clock",
			position: "top_left"
		},
//		{
//			module: "calendar",
//			header: "US Holidays",
//			position: "top_right",
//			config: {
//				calendars: [
//					{
//						symbol: "calendar-check",
//						url: "webcal://www.calendarlabs.com/ical-calendar/ics/76/US_Holidays.ics"					}
//				]
//			}
//		},
//		{
//			module: "compliments",
//			position: "lower_third"
//		},
//		{
//			module: "currentweather",
//			position: "top_right",
//			config: {
//				location: "New York",
//				locationID: "", //ID from http://bulk.openweathermap.org/sample/city.list.json.gz; unzip the gz file and find your city
//				appid: "YOUR_OPENWEATHER_API_KEY"
//			}
//		},
//		{
//			module: "weatherforecast",
//			position: "top_right",
//			header: "Weather Forecast",
//			config: {
//				location: "New York",
//				locationID: "5128581", //ID from http://bulk.openweathermap.org/sample/city.list.json.gz; unzip the gz file and find your city
//				appid: "YOUR_OPENWEATHER_API_KEY"
//			}
//		},
		{
			module: "newsfeed",
			position: "top_center",
			config: {
				feeds: [
					{
						title: "New York Times",
						url: "http://www.nytimes.com/services/xml/rss/nyt/HomePage.xml"
					}
				],
				showSourceTitle: true,
				showPublishDate: true,
				broadcastNewsFeeds: true,
				broadcastNewsUpdates: true
			}
		},
		{
			module: 'MMM-Api'
		},
		{
			module: 'MMM-SmartWebDisplay',
			position: 'fullscreen_above',	// This can be any of the regions.
			config: {
				// See 'Configuration options' for more information.
				logDebug: false, //set to true to get detailed debug logs. To see them : "Ctrl+Shift+i"
				height:"100%", //hauteur du cadre en pixel ou %
				width:"100%", //largeur
						updateInterval: 0, //in min. Set it to 0 for no refresh (for videos)
						NextURLInterval: 0, //in min, set it to 0 not to have automatic URL change. If only 1 URL given, it will be updated
						displayLastUpdate: false, //to display the last update of the URL
						//displayLastUpdateFormat: 'ddd - HH:mm:ss', //format of the date and time to display
						url: ["./modules/MMM-SmartWebDisplay/public/Assistants.html"], //source of the URL to be displayed
						scrolling: "no", // allow scrolling or not. html 4 only
						shutoffDelay: 10000 //delay in miliseconds to video shut-off while using together with MMM-PIR-Sensor 
				}
		},
		{
			module: 'MMM-Remote-Control'
			// uncomment the following line to show the URL of the remote control on the mirror
			// , position: 'bottom_left'
			// you can hide this module afterwards from the remote control itself
		},
	]

};

/*************** DO NOT EDIT THE LINE BELOW ***************/
if (typeof module !== "undefined") {module.exports = config;}
