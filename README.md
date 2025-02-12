# WebScraper

## 1: About
The WebScraper scrapes websites for links via extracting and parsing HTML data on HTTPS websites using SLL and TCP/IP. In doing so, we extract any links that may appear on the site and conduct so efficiently and timely. I was inspired to create such a simple yet complex program from my work as a Web VEO, where I scraped websites and scoured for voting data in the 2024 election. Here, I made the process less time-consuming and allowed for all links to be shown.

## 2: Instructions

Ensure the usage and implementation of the Boost/Beast library that enables networking protocols and the usage of web sockets to obtain HTML data.

Also, include the flag -lss when compiling as G++ or using an ordinary compiler like onlineGDB. Ensure you include the appropriate libraries and connect properly with the client and server to avoid any errors.

## 3: Usage/Application

As of right now, applications can be further made to implement a database-like destination, possibly through SQL or perhaps output is a CSV file. Furthermore, we may be able to clean the data further in case we have links that aren't useful or irrelevant to what we are searching for. We may also introduce threading/parallelization to increase throughput and apply various websites to obtain useful data.

