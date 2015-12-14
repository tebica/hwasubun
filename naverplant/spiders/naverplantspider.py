# -*- coding: utf-8 -*-
import scrapy
# from scrapy.linkextractors import LinkExtractor
from scrapy.spiders import CrawlSpider, Rule
# import sys
# print sys.path
from naverplant.items import NaverplantItem


class GetplantSpider(CrawlSpider):
    name = 'naverplantspider'
    # allowed_domains = ['http://terms.naver.com/list.nhn?cid=46676&categoryId=46676&page=1']
    # start_urls = ['http://terms.naver.com/list.nhn?cid=42526&categoryId=44534&page=1/']
    start_urls = []
    for i in range(1, 2):
        start_urls.append("http://terms.naver.com/list.nhn?cid=42526&categoryId=44534&page="+str(i))
#    print "--------"+start_urls[2]
#    rules = (
#        Rule(LinkExtractor(allow=r'Items/'), callback='parse_item', follow=True),
#    )

#    def parse_item(self, response):
#        for href in response.css("ul.directory.dir-col > li > a::attr('href')"):
#            url = response.urljoin(href.extract())
#            yield scrapy.Request(url, callback=self.parse_dir_contents)

    def parse(self, response):
        for href in response.css("ul.thmb_lst > li > span > a::attr('href')"):
            url = response.urljoin(href.extract())
            yield scrapy.Request(url, callback=self.parse_dir_contents)

    def parse_dir_contents(self, response):
       # for sel in response.xpath('//div[@class="section_wrap"]/li'):




        for sel in response.css('div.section_wrap > div.h_group'):
            item = NaverplantItem()
            item['name'] = sel.xpath('//h2/text()').extract()[0]
            item['content'] = sel.xpath('//div[@id="size_ct"]').extract()[0] #.encode('utf-8')
            #item['content'] = sel.css('div#size_ct"').extract()[0].encode('utf-8')
            #open('/tmp/naver.plant.debug', 'a').write(repr(item['content']) + '\n')
            print item['name'][0]
#            item['link'] = sel.xpath('a/@href').extract()
#            item['desc'] = sel.xpath('text()').extract()
#               i = NaverplantItem()
            yield item
