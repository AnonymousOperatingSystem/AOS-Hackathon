import { chain } from "mathjs";
import { Decimal } from "decimal.js";

const formatDate = (data, formate = "yyyy-MM-dd hh:mm:ss") => {
  if (isEmpty(data)) {
    return "";
  }
  console.log(data);
  let fmt = formate;
  const date = new Date(data || "");
  const o = {
    "M+": date.getMonth() + 1, // 月份
    "d+": date.getDate(), // 日
    "h+": date.getHours(), // 小时
    "m+": date.getMinutes(), // 分
    "s+": date.getSeconds(), // 秒
    "q+": Math.floor((date.getMonth() + 3) / 3), // 季度
    S: date.getMilliseconds(), // 毫秒
  };
  if (/(y+)/.test(fmt)) {
    fmt = fmt.replace(
      RegExp.$1,
      `${date.getFullYear()}`.substr(4 - RegExp.$1.length)
    );
  }
  for (const k in o) {
    if (new RegExp(`(${k})`).test(fmt)) {
      fmt = fmt.replace(
        RegExp.$1,
        RegExp.$1.length === 1 ? o[k] : `00${o[k]}`.substr(`${o[k]}`.length)
      );
    }
  }
  return fmt;
};

function timestampToDate(timestamp, formats) {
  // formats格式包括
  // 1. Y-m-d
  // 2. Y-m-d H:i:s
  // 3. Y年m月d日
  // 4. Y年m月d日 H时i分
  formats = formats || "Y-m-d";

  var zero = function (value) {
    if (value < 10) {
      return "0" + value;
    }
    return value;
  };

  var myDate = timestamp ? new Date(timestamp) : new Date();

  var year = myDate.getFullYear();
  var month = zero(myDate.getMonth() + 1);
  var day = zero(myDate.getDate());

  var hour = zero(myDate.getHours());
  var minite = zero(myDate.getMinutes());
  var second = zero(myDate.getSeconds());

  return formats.replace(/Y|m|d|H|i|s/gi, function (matches) {
    return {
      Y: year,
      m: month,
      d: day,
      H: hour,
      i: minite,
      s: second,
    }[matches];
  });
}

function isEmpty(obj) {
  if (
    obj === "null" ||
    obj === null ||
    obj === "(null)" ||
    obj === undefined ||
    obj === "" ||
    obj.length === 0
  ) {
    return true;
  }
  return false;
}

function percentNum(num1, num2) {
  const percent = Math.round((num1 / num2) * 10000) / 100.0;
  return percent === Infinity || percent > 100 ? "100%" : percent + "%";
}

function percentCount(num1, num2) {
  const percent = Math.round((num1 / num2) * 10000) / 100.0;
  return percent === Infinity || percent > 100 ? 100 : percent;
}

function getQueryString(name) {
  var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)", "i");
  var r = window.location.search.substr(1).match(reg);
  if (r != null) return unescape(r[2]);
  return null;
}
//计算apy
function computerAPY(headTime, startTime, qua, a, fade_factor) {
  // a*(1-b%)^[(t2-t1)/60/60/24-1]

  // 1 个质押物（1AOS 或 1ASP）的实时占比为x%
  // 矿池开始时间 t1
  // 当前区块时间 t2
  // 下一次释放的总量 y=a*(1-b%)^([t2-t1)/60/60/24-1]
  // APY=x% * y * 6 * 24 * 365 * 0.8
  var timestamp = parseInt(new Date(headTime).getTime() / 1000) + 8 * 60 * 60;
  // console.log(timestamp,startTime,qua,a,fade_factor)
  const pow = Math.max(0, (timestamp - startTime) / 60 / 60 / 24 - 1);
  let y = chain(1 - fade_factor / 100)
    .pow(pow)
    .multiply(a)
    .done();
  // console.log(percent,y,day)
  let x = new Decimal(1)
    .dividedBy(new Decimal(1 + parseFloat(qua)))
    .mul(y)
    .mul(6)
    .mul(24)
    .mul(365)
    .mul(0.8)
    .mul(1000);
  // return new Decimal(percent) (chain(percent).multiply(y).multiply(6).multiply(24).multiply(day).multiply(0.8).multiply(100).done()).toFixed(0)
  return x.toFixed(0);
}

//计算apy
function computerAmout() {
  // a=x*b%/[144*[1-(1-b%)^365]]
  const pow = chain(1 - 0.05)
    .pow(14)
    .done();
  const b = chain(144)
    .multiply(1 - pow)
    .done();
  return chain(6000000).multiply(0.05).divide(b).done();
  // var timestamp = parseInt(new Date(headTime).getTime()/1000);
  // let y = chain(1- fade_factor / 100).pow(
  // Math.trunc((timestamp-startTime)/60/60/24 - 1)).done()
  // y = y * a;
  // // x% * y * 6 * 24 * 365 * 0.8
  // const day = Math.trunc(parseInt(duration) / 60 / 60 / 24 )
  // return Math.trunc(chain(percent).multiply(y).multiply(6 * 24 * day * 0.8).done())
}

export {
  formatDate,
  isEmpty,
  getQueryString,
  percentNum,
  computerAPY,
  percentCount,
  timestampToDate,
  computerAmout,
};
