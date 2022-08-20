
var QQMapWX = require('../../utils/qqmap-wx-jssdk.js');
var that=this;
// 实例化API核心类
var qqmapsdk = new QQMapWX({
    key: 'CU6BZ-KCQ6D-7Q74Q-PRZ7L-WGCBZ-4MFG4' // 必填
});
const key = 'CU6BZ-KCQ6D-7Q74Q-PRZ7L-WGCBZ-4MFG4'; //使用在腾讯位置服务申请的key
const referer = '智能拐杖'; //调用插件的app的名称
const location = JSON.stringify({
  latitude: 39.89631551,
  longitude: 116.323459711
});
const category = '生活服务,娱乐休闲';
const chooseLocation = requirePlugin('chooseLocation');
Page({
    data:{
        s_latitude:'39.89631551',
        s_longitude:'116.323459708',
        Radius:''
    },
    Remide() {
        wx.showModal({
            title: '温馨提示',
            content: '超出围栏范围！',
            success: function (res) {
              if (res.confirm) {
              } else if (res.cancel) {
              }
            }
          })
    },
    onReady(){
        wx.navigateTo({
            url: 'plugin://chooseLocation/index?key=' + key + '&referer=' + referer + '&location=' + location + '&category=' + category
          });
    },
    onShow () {
        const location1 = chooseLocation.getLocation(); // 如果点击确认选点按钮，则返回选点结果对象，否则返回null
        console.log(location1);
        this.setData({
            s_latitude:location1.latitude,
            s_longitude:location1.longitude
        })
        console.log(this.data.s_latitude);
    },
    onUnload () {
        // 页面卸载时设置插件选点数据为null，防止再次进入页面，geLocation返回的是上次选点结果
        chooseLocation.setLocation(null);
        const location1 = chooseLocation.getLocation(); // 如果点击确认选点按钮，则返回选点结果对象，否则返回null
        console.log(location1);
        this.setData({
            s_latitude:location1.latitude,
            s_longitude:location1.longitude
        })
        console.log(this.data.s_latitude);
    },
//在Page({})中使用下列代码
//事件触发，调用接口
formSubmit(e){
    var _this = this;
    //调用距离计算接口
    qqmapsdk.calculateDistance({
        //获取表单提交的经纬度并设置from和to参数（示例为string格式）
        from:{
            latitude:this.data.s_latitude,
            longitude:this.data.s_longitude
        }, //若起点有数据则采用起点坐标，若为空默认当前地址
        to:'39.89631551,116.323559675', //终点坐标
        success: function(res) {//成功后的回调
          console.log(res);
          var res = res.result;
          var dis = [];
          for (var i = 0; i < res.elements.length; i++) {
            dis.push(res.elements[i].distance); //将返回数据存入dis数组，
          }
          _this.setData({ //设置并更新distance数据
            distance: dis,
          });
          console.log(this);
        },
        fail: function(error) {
          console.error(error);
        },
        complete: function(res) {
          console.log(res);
        }
    });
}
})
