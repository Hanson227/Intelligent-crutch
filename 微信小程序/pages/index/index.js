// 引入SDK核心类
var QQMapWX = require('../../utils/qqmap-wx-jssdk.js');

// 实例化API核心类
var qqmapsdk = new QQMapWX({
  key: 'CU6BZ-KCQ6D-7Q74Q-PRZ7L-WGCBZ-4MFG4' // 必填
});
Page({
//在Page({})中使用下列代码
//触发表单提交事件，调用接口
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
    location: e.detail.value.reverseGeo || '', //获取表单传入的位置坐标,不填默认当前位置,示例为string格式
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
}
})