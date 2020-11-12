import AppRouter from "./route/AppRouter";
import intl from "react-intl-universal";
import { useEffect, useState } from "react";
import { isEmpty, getQueryString } from "./utils";
const locales = {
  en: require("./locales/en-US.json"),
  cn: require("./locales/zh-CN.json"),
  kr: require("./locales/ko-KR.json"),
};

function App() {
  const [initDone, setInitDone] = useState(false);
  useEffect(() => {
    loadLocales();
  }, []);
  function loadLocales() {
    // init method will load CLDR locale data according to currentLocale
    // react-intl-universal is singleton, so you should init it only once in your app
    let lang = "cn";
    const system = getQueryString("lang");
    const supports = ["en", "cn", "kr"];
    if (!isEmpty(system) && supports.indexOf(system) > -1) {
      lang = system;
    }
    sessionStorage.setItem("lang", lang);
    setInitDone(false);
    intl
      .init({
        currentLocale: lang, // TODO: determine locale here
        locales,
      })
      .then(() => {
        // After loading CLDR locale data, start to render
        setInitDone(true);
      });
  }

  return initDone && <AppRouter />;
}

export default App;
