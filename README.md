# The Easiest Way to Send Data from ESP8266 to a Webpage

Find the full article on [Instructable](https://www.instructables.com/The-Easiest-Way-to-Send-Data-From-ESP8266-to-a-Web/) and [Arduino ProjectHub](https://create.arduino.cc/projecthub/AbdullahMahboob/the-easiest-way-to-send-data-from-esp8266-to-a-website-b8a348).

There are hundreds of real-world applications that involve sending data from a microcontroller to a website, webpage, or web dashboard. And almost all of such applications prefer to be wireless. In such cases, the ESPxxxx family of WiFi modules is the most commonly used one due to their tiny form factor as compared to Arduinos coupled with WiFi shield.

But communication of any kind of data wirelessly between two things requires the knowledge of the languages of the internet, like HTTP GET and POST and XML or JSON. This is usually out of scope of embedded engineers like us, and their mere mention is sometimes enough to send shivers down our spines. So I'm going to try to kick this fear's ass today, with a much simpler and more powerful way to do the same thing, and more. Using a tool I call Grandeur.
It lets you send data to and fro among your devices and webpages without a learning curve or a requirement to know the languages of the web, e.g., HTTP, JSON, XML, WebSockets, MQTT, etc. Let me show you how. Follow me to the end!

## Step 1: Program ESP8266 To Send Data

Here's the code I used for my ESP8266:

```cpp
#include <Grandeur.h>
#include "WiFi.h"

// WiFi credentials
const char* ssid = YourWiFiSsid;
const char* passphrase = YourWiFiPassphrase;

// Grandeur credentials
const char * apiKey = YourApiKey;
const char* token = YourDeviceToken;
const char* deviceId = YourDeviceId;

Grandeur::Project project;

void setup() {
  Serial.begin(9600);
  // This connects to the device to WiFi.
  connectToWiFi(ssid, passphrase);
  // You can initialize device configurations like this.
  project = grandeur.init(apiKey, token);
}

unsigned long current = millis();

void loop() {
  // This sends data to internet.
  if(project.isConnected() && millis() - current > 500) {
    project.device(deviceId).data().set("millis", millis());

    current = millis();
  }

  // This runs the SDK when the device WiFi is connected.
  if(WiFi.status() == WL_CONNECTED) project.loop();
}
```

`<Grandeur.h>` gives us access to the functions to send and receive data from the internet.

In `setup()`, I connect to WiFi using the `connectToWiFi()` function and then connect to internet using the `grandeur.init()` function.

In `loop()`, I send the `millis()` timer value to the internet using `project.device(deviceId).data().set("millis", millis())`. The if-statement

```cpp
if(millis() - current > X) {
      /** CODE
      */
      current = millis();
}
```

makes sure the CODE inside the if-statement runs only after every X milliseconds. This is a way of introducing a nonblocking `delay(X)` into the code. This keep my ESP8266 from crashing by sending data too fast.

Here's the WiFi.h file that I included:

```cpp
#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>

void connectToWiFi(const char* ssid, const char* passphrase) {
 Serial.begin(9600);
  // Disconnecting WiFi if it"s already connected.
 WiFi.disconnect();
  // Setting it to Station mode which basically scans for nearby WiFi routers.
 WiFi.mode(WIFI_STA);
  // Begin connecting to WiFi
 WiFi.begin(ssid, passphrase);
 Serial.printf("\nDevice is connecting to WiFi using SSID %s and Passphrase %s.\n", ssid, passphrase);
}

#endif /* WIFI_H */
```

It gives us the connectToWiFi() function that helps us connect with WiFi.

## Step 2: Build A Simple Webpage

Here's a simple webpage that I designed. It displays any data that's coming from my ESP8266.

```html
<html>
     <head>
         <title>Receiving data from ESP8266</title>
         <!-- Link to Grandeur -->
         <script src="https://unpkg.com/grandeur-js"></script>
         <link rel="stylesheet" href="./main.css">
     </head>
     <body>
         <h1>Receiving data from ESP8266 &nbsp</h1>
         <h1 class="loader">/</h1>
         <p id="data">–</p>

         <script src="./index.js"></script>
     </body>
</html>
```

The line 5 (`script` tag) does the same thing in this webpage as `<Grandeur.h>` does in the ESP8266 program – gives us access to the functions to send and receive data from the internet.

Here's the stylesheet I used, it makes the loader rotate and brings everything to the center:

```css
body {
    display: flex;
    flex-direction: column;
    align-items: center;
}

/* Loader rotation */
.loader {
    position: relative;
    -webkit-animation: spin 1=0.5s linear infinite;
    -moz-animation: spin 0.5s linear infinite;
    animation: spin 0.5s linear infinite;
}

@-moz-keyframes spin { 
    100% { -moz-transform: rotate(360deg); } 
}
@-webkit-keyframes spin { 
    100% { -webkit-transform: rotate(360deg); } 
}
@keyframes spin { 
    100% { 
         -webkit-transform: rotate(360deg); 
         transform:rotate(360deg); 
    } 
}

/* End loader rotation */

#data {
    font-size: 5em;
    margin-block: 0;
}
```

## Step 3: Program The Webpage To Receive Data

Here is the Javascript file of my page:

```js
const apiKey = YourApiKey;
const accessKey = YourAccessKey;
const accessToken = YourAccessToken;

const project = grandeur.init(apiKey, accessKey, accessToken);

project.auth().login(YourUserEmail, YourUserPassword);

// This subscribes to the "millis" variable.
project.devices().device(YourDeviceId).data().on("millis", (path, value) => document.getElementById("data").innerHTML = value);
```

Subscribing to a variable from my webpage means that my webpage is asking to receive the variable's value when my ESP8266 sends it to the internet. Here I'm asking to receive whatever my ESP8266 sends inside the "millis" variable, which in my case – according to the Step 1 – is the ESP8266's `millis()` timer value.

## Step 4: Fill In The Gaps

Now that the ESP8266 is ready to send data and the webpage is ready to receive it, we need to fill in some gaps.

In ESP8266 program, we need three things:

1. YourApiKey
2. YourDeviceToken
3. YourDeviceId


In our webpage's Javascript, we need six things:

1. YourApiKey
2. YourAccessKey

3. YourAccessToken
4. YourUserEmail
5. YourUserPassword
6. YourDeviceId

To get these, we need to create a project on the Grandeur Dashboard at https://cloud.grandeur.tech/. Here are the steps you can follow:

1. Go to https://cloud.grandeur.tech/ and click on get access to sign up. ![Dashboard – signing up](https://user-images.githubusercontent.com/26253942/150967428-4a0893a5-6c02-4623-a063-fd30b2339055.gif)

2. Create a **new project**. ![Dashboard – creating a new project](https://user-images.githubusercontent.com/26253942/150967606-3c06d8e8-df5e-40ca-a309-b2e5547490aa.gif)

3. Add a **new model**. ![Dashboard – registering a new model](https://user-images.githubusercontent.com/26253942/150967617-08b35321-98d9-4c4f-857d-da709b223285.gif)

4. Add a **new device**. Copy and save the device ID and token it returns. ![Dashboard – registering a new device](https://user-images.githubusercontent.com/26253942/150967623-cdedb74c-4bf8-4aed-b9bb-99f0417e5c61.gif)

5. Add a **new user**. Copy and save the user email and password you used here. ![Dashboard – registering a new user gif](https://user-images.githubusercontent.com/26253942/150967635-dc26c432-499b-4b4f-9dea-b46258cd8c7a.gif)

6. **Pair the device** we created in step 4 with the user we created in step 5. ![Dashboard – pairing device to a user](https://user-images.githubusercontent.com/26253942/150967647-90b59379-91bc-499d-aa10-6e85ff456817.gif)

7. Go to settings, **copy and save the API key** from here. ![Dashboard – api key](https://user-images.githubusercontent.com/26253942/150967650-4ed84602-1597-41c0-94e0-10f1d145814d.gif)

8. Generate **new access key**. Copy and save the access key and token it returns. ![Dashboard – generating access credentials](https://user-images.githubusercontent.com/26253942/150967670-3c87cf26-f674-47e2-95fd-e91f342e0719.gif)

9. **Allow the origin** http://localhost:8080 or whatever URL your webpage is served on. We'll see where to find this URL in the next step. ![Dashboard – allowing an origin](https://user-images.githubusercontent.com/26253942/150967677-227cd808-3b8f-4eba-861f-08013f406cad.gif)


By doing this, you get access to use Grandeur as your communication broker.

Now,

1. In ESP8266 program, paste your WiFi SSID and WiFi passphrase, and the API key, device ID, and device token, you copied in step 7 and 4, respectively.
2. In the webpage's Javascript file, paste the API key, access key, access token, user email, user password, and device ID, you copied in step 7, 8, 5, and 4, respectively.

Now both the ESP8266 and the webpage are ready to go live.

## Step 5: Go Live

Compile and upload the ESP8266 program in your ESP8266 using Arduino IDE or CLI.

Start a local server to serve your webpage. If you have python 3, you can run this command in a terminal in your webpage's folder:

```bash
python3 -m http.server
```

If you have node.js, you can run this:

```bash
npx http-server
```

Open your browser and go to the URL that is displayed in your terminal after running these commands. This is the URL that you should add while allowing origin in the last step.

If everything works perfectly, your ESP8266 will connect to WiFi, and then to the internet, and start sending `millis()` value twice in a second. In your browser, you should see the wheel spinning and below it, the value of ESP8266's millis updating twice every second.

## Step 6: Conclusion

In essence, it's just two methods that are making this work:

1. In ESP8266, `project.device(deviceId).data().set("millis", millis())` sends data to the internet.
2. In the webpage's Javascript file, `project.devices().device(deviceId).data().on("millis", FunctionToPrintData)` receives data from the internet.

You can also send data in reverse, from the webpage to the ESP8266, by switching the methods:

1. In the webpage's Javascript file, `project.devices().device(deviceId).data().set("hello", "Hello from webpage.")`.
2. In ESP8266, `project.device(deviceId).data().on("hello", FunctionToPrintData)`.

You can also receive and send data to more than one device from the webpage. Just add a new device and pair it with your user in your project from the Grandeur Dashboard at https://cloud.grandeur.tech/devices/ and copy and paste the line `project.devices().device(deviceId).data().on("millis", FunctionToPrintData)` another time in the webpage's Javascript but replace its device Id with the new device, and now your webpage will receive data from both the ESP8266. This is how the webpage's Javascript looks for receiving data from multiple devices:

```js
const apiKey = YourApiKey;
const accessKey = YourAccessKey;
const accessToken = YourAccessToken;

const project = grandeur.init(apiKey, accessKey, accessToken);

project.auth().login(YourUserEmail, YourUserPassword);

// Receiving data from device 1.
project.devices().device(YourDeviceIdOne).data().on("millis", (path, value) => document.getElementById("data1").innerHTML = value);

// Receiving data from device 2.
project.devices().device(YourDeviceIdTwo).data().on("millis", (path, value) => document.getElementById("data2").innerHTML = value);
```

I hope this simplifies the data exchange among your devices and webpages. If you have any questions, ask them in comments, or you can always reach me out at ma@grandeur.tech. Thanks for bearing with me! 
