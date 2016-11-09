# Apache httpd 2.4.x Module for a CMS based on MongoDB (mod_mongocms)

Have you ever thought about having a CMS module running directly within the Apache httpd context?  
I needed a web data storage which is fast and good accessible, which leads to the MongoDB as a persistence layer.
For security and performance reasons the Apache module it is more or less a facade in front of the MongoDB. It checks all inbound traffic to ensure only data which shall be stored, will be stored 
at the right place.
  
**Table of Contents**
* [Installation Instruction](documentation/installation.md)
* [Commands](documentation/commands.md)
* [Configuration](documentation/configuration.md)
