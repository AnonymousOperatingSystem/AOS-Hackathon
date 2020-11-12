import Eos from "eosjs";
import {
  RPCBASE,
  scatterNetwork,
  ASSETS_CONTRACT,
} from "./config";

const config = {
  httpEndpoint: RPCBASE,
  chainId: scatterNetwork.chainId, // 通过cleos get info可以获取chainId
  expireInSeconds: 60,
};

const eos = Eos(config);

async function getCurrentBalance(account: string, contract: string, symbol: string) {
  return await eos.getCurrencyBalance(contract, account, symbol);
}

async function getBlockInfo() {

  return await eos.getInfo({});
}

async function getPools() {
    const data = {
      "code": ASSETS_CONTRACT,  // 本合约账户
        "json": true,
        "limit": -1,
        "lower_bound": "",
        "scope": ASSETS_CONTRACT, // 本合约账户
        "table": "globals",         // 固定写死
        "table_key": "",
        "upper_bound": ""
    }
    return await eos.getTableRows(data)
}

//获取每一个矿池的信息
async function getPoolDetail(poolID: number) {

  const data =  {
    "code": ASSETS_CONTRACT,// 本合约账户
    "json": true,
    "limit": -1,
    "lower_bound": "",
    "scope": poolID,         // 矿池id，int类型
    "table": "pools",         // 固定写死
    "table_key": "",
    "upper_bound": ""
}
return await eos.getTableRows(data);
}
async function checkSymbol(symbol: string) {
  const data = {
    json: true,
    code: ASSETS_CONTRACT,
    table: "stat",
    scope: symbol,
  };
  return await eos.getTableRows(data);
}

async function getUserInfo(account: string) {
  const data = {
    "code": ASSETS_CONTRACT,// 本合约账户
    "json": true,
    "limit": -1,
    "lower_bound": "",
    "scope": account,       // 用户名
    "table": "users",         // 固定写死
    "table_key": "",
    "upper_bound": ""
}
return await eos.getTableRows(data);
}

export { getCurrentBalance, getBlockInfo, checkSymbol,getPools,getPoolDetail,getUserInfo };
