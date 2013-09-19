# Copyright (C) 2013 Misha Nasledov <misha@nasledov.com>
# All Rights Reserved.
import re
import time
import mechanize
import urllib
import sys
from bs4 import BeautifulSoup

def scrapePosts(br, postLinks):
    posts = []
    for postLink in postLinks:
        post = scrapeTimelinePost(br, postLink)
        posts.append(post)
        print getPostXML(post)
        time.sleep(3)
    return posts

def scrapeTimelinePost(br, postURL):
    ret = {}

    print >> sys.stderr, "Scraping "+postURL

    br.open(postURL)
    resp = br.response()

    htmlBody = resp.read()
    soup = BeautifulSoup(htmlBody)

    postTextSpan = soup.find_all('span', class_='userContent')
    if len(postTextSpan) > 0:
        postText = postTextSpan[0].get_text()
        ret['text'] = postText

    linkBlock = soup.find_all('a', class_='pam shareText')
    if len(linkBlock) > 0:
        ## Find the link URL in the onmouseover block
        linkURL = re.search('(http(s?).*)"', linkBlock[0]['onmouseover']).group(1)
        ## Remove backslashes
        linkURL = re.sub(r'\\', '', linkURL)
        ret['link_url'] = linkURL

    postImage = soup.find_all('img', class_='shareMediaPhoto img')
    if len(postImage) > 0:
        ## The image is ref'd in the CSS style
        postImageStyle = postImage[0]['style']
        ## Use regex to find the image and remove URL encoding
        postImageURL = urllib.unquote(re.search(r'(url|src)=(http(s?)[^&]+)(&|\))', postImageStyle).group(2))
        ret['image'] = postImageURL

    postDateBlock = soup.find_all('abbr')[0]
    ## The date has a formatted string and a UNIX timestamp
    postUTime = postDateBlock['data-utime']
    postDate = postDateBlock['title']

    ret['date_fmt'] = postDate
    ret['date_utime'] = postUTime

    return ret

def getPostXML(post):
    itemXML = "<item>\n"
    itemXML += "  <pubDate>"+post['date_fmt']+"</pubDate>\n"
    itemXML += "  <title>"+post['date_fmt']+"</title>\n";
    contentXML = "  <content:encoded>"
    if post.has_key('text'):
        contentXML += post['text']
    if post.has_key('image'):
        contentXML += "<br/>Image: <img src='"+post['image']+"'/>";
    if post.has_key('link_url'):
        contentXML += "<br/>Link: <a href='"+post['link_url']+"'>Link</a>";
    contentXML = contentXML.replace('\n', '');
    contentXML += "</content:encoded>\n";
    itemXML += contentXML + "</item>\n";
    return itemXML.encode("utf-8")


## Main code:
br = mechanize.Browser()
br.set_handle_robots(False)

postURLs = [
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/220284061385346',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/219966181417425',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/203682949720845',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/198559480231304',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/148767168559788',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/196011253816071',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/143305695773353',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/223589964377073',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/315851401763742',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/308290615866908',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/307026192660017',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/304639572898679',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/297665740245428',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/386466544743684',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/376286392426699',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/465796990116269',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/323715397709624',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/243353335773154',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/361605683893195',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/434753406549797',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/377422608959480',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/463985213618510',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/440994415929860',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/440991215930180',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/261836857247097',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/407947989239803',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/210892939014245',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/224690014298762',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/365689790139536',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/334808486580118',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/175805435873492',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/201663233277637',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/190254274424086',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/263489623736766',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/366871193335758',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/158186144302089',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/387900411239261',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/318316638224775',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/329252963787249',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/371897142839588',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/170484899729350',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/365203510159007',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/364154946929122',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/319184668124962',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/237482302995411',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/189067541190755',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/220284061385346',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/219966181417425',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/572529632762187',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/552052021490765',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/561406907218842',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/560968263919734',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/491218487567639',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/243595542435416',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/112627665564546',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/432090260171504',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/377732122308642',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/369100949840142',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/359645390794126',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/525796790782955',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/164502873690692',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/370633146355223',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/182882945170092',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/178233045634795',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/172818622855258',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/357334841015114',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/416877145043737',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/358263664252675',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/411789392210871',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/253750268061100',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/428013790582255',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/260752317373170',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/161666873969700',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/303384056424991',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/386466544743684',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/657841170911849',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/652359191460047',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/647715755257724',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/645204372175529',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/643685125660787',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/636198139742819',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/632704613425505',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/629073463788620',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/626603614035605',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/626552454040721',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/625643874131579',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/624132307616069',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/623734464322520',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/623342324361734',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/623341881028445',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/619739381388695',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/262331967236887',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/541079489277546',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/349512768503672',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/427090407384382',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/572592839419012',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/595370977158869',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/594364160592884',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/296555440470460',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/483554298367840',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/547809695237884',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/193081197500765',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/580261965334390',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/324483080990309',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/141757502646891',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/701290139900285',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/697557270273572',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/694048240624475',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/692117287484237',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/690397297656236',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/687851091244190',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/685675081461791',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/681710191858280',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/678939758801990',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/673626522666647',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/671906866171946',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/671325346230098',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/670855799610386',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/670327959663170',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/670010483028251',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/666166373412662',
    'https://www.facebook.com/ThinkBeforeYouBounce/posts/657841170911849']

posts = scrapePosts(br, postURLs)

#for post in posts:
#   print getPostXML(post)
