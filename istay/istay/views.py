#-*- coding: utf-8 -*-
from django.views.generic import TemplateView
from django.shortcuts import render
from parse_rest.connection import register
register('atDE32qTfF0GLErm2GCmuDQBxmb4IJ90pcDZyXQX', 'mDG7RMafnOak6woeYcqbu8Oys9NDOZ340iL34u2d')
from parse_rest.datatypes import Object
from django.http import HttpResponse

class nongsaro(Object):
    pass

class Usert(Object):
    pass

class ListPlantView(TemplateView):
    template_name = "list.html"
    def get(self, request):
        _key = request.GET.get('key')
        result = nongsaro.Query.filter(name__regex=_key)

        return render(request, self.template_name, {'param' : result})


class DetailPlantView(TemplateView):
    template_name = "detail.html"
    def get(self, request):
        _key = request.GET.get('key')
        result = nongsaro.Query.filter(objectId=_key)

        return render(request, self.template_name, {'param' : result,
                                                    'id':_key})

class SaveView(TemplateView):
    template_name = "save.html"
    def get(self, request):

        userss = Usert()
        infos = {'plantid':''}

        v_userid = request.GET.get('userid')
        v_plantid = request.GET.get('plantid')
        userss.userid = v_userid
        userss.plantid = v_plantid
        userss.save()

        infos['plantid'] = v_plantid
        infos['plantname'] = request.GET.get('plantname')
        infos['plantpic'] = request.GET.get('plantpic')
        infos['username'] = request.GET.get('username')

        return render(request, self.template_name, {'param' : infos})