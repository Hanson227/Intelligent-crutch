// 引入SDK核心类
import mqtt from '../../utils/mqtt.js'
var QQMapWX = require('../../utils/qqmap-wx-jssdk.js');
var qqmapsdk = new QQMapWX({
  key: 'CU6BZ-KCQ6D-7Q74Q-PRZ7L-WGCBZ-4MFG4' // 必填
});
var client = null;

Page({
  data: {
    currentValue: 0,
    subTopic: "/iot/1620/sub/123",//小程序订阅
    pubTopic: "/iot/1620/pub/123",//esp订阅
    w:'',
    j:''
  },
  onShow() {
    this.connectMqtt()
  },
  connectMqtt() {
    const options = {
      connectTimeout: 4000,
      clientId: 'mp' + Math.ceil(Math.random() * 100),
      port: 8084,
      username: '18e6c7ce077a9e94c3ebd20284f925cf',
      password: '123123',
    }

    client = mqtt.connect('wxs://t.yoyolife.fun/mqtt', options)
    client.on('reconnect', (error) => {
      console.log('正在重连:', error)
    })

    client.on('error', (error) => {
      console.log('连接失败:', error)
    })
    let that = this
    client.on('connect', (e) => {
      console.log('成功连接服务器')
      client.subscribe(that.data.subTopic, { qos: 0 }, function (err) {
        if (!err) {
          client.publish(that.data.pubTopic, 'login')
        }
      })
    });
    client.on('message', function (topic, message) {
      let tem = {}
      tem = JSON.parse(message)
      that.setData({
       w:tem.weidu,
       j:tem.jindu
      })
      console.log(tem)
      console.log(message.toString())
    })
  },
  onDrag(event) {
    this.setData({
      currentValue: event.detail.value,
    });
    client.publish(this.data.pubTopic, event.detail.value.toString())
  },
  formSubmit(e) {
    var _this = this;
    qqmapsdk.reverseGeocoder({
      //位置坐标，默认获取当前位置，非必须参数
      /**
       * 
       //Object格式
        location: {
          latitude: 39.984060,
          longitude: 116.307520
        },
      */
      /**
       *
       //String格式
        location: '39.984060,116.307520',
      */
      location: {
        latitude: this.data.w||39.89631551,//||,//用this语法来进行数据绑定
        longitude:this.data.j||116.323559675,//this.data.j
      } ,//获取表单传入的位置坐标,不填默认当前位置,示例为string格式
      //get_poi: 1, //是否返回周边POI列表：1.返回；0不返回(默认),非必须参数
      success: function (res) {//成功后的回调
        console.log(res);
        var res = res.result;
        var mks = [];
        /**
         *  当get_poi为1时，检索当前位置或者location周边poi数据并在地图显示，可根据需求是否使用
         *
            for (var i = 0; i < result.pois.length; i++) {
            mks.push({ // 获取返回结果，放到mks数组中
                title: result.pois[i].title,
                id: result.pois[i].id,
                latitude: result.pois[i].location.lat,
                longitude: result.pois[i].location.lng,
                iconPath: './resources/placeholder.png', //图标路径
                width: 20,
                height: 20
            })
            }
        *
        **/
        //当get_poi为0时或者为不填默认值时，检索目标位置，按需使用
        mks.push({ // 获取返回结果，放到mks数组中
          title: res.address,
          id: 0,
          latitude: res.location.lat,
          longitude: res.location.lng,
          iconPath: './resources/placeholder.png',//图标路径
          width: 20,
          height: 20,
          callout: { //在markers上展示地址名称，根据需求是否需要
            content: res.address,
            color: '#000',
            display: 'ALWAYS'
          }
        });
        _this.setData({ //设置markers属性和地图位置poi，将结果在地图展示
          markers: mks,
          poi: {
            latitude: res.location.lat,
            longitude: res.location.lng
          }
        });
      },
      fail: function (error) {
        console.error(error);
      },
      complete: function (res) {
        console.log(res);
      }
    })
  },
})