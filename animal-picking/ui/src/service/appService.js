import { APPBASE } from "./config";
import { create } from "apisauce";
// define the api
const api = create({ baseURL: APPBASE });

api.addRequestTransform((request) => {
  request.headers["lang"] = sessionStorage.getItem("lang");
});
