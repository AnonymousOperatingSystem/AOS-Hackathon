package plus.aos.jaos.impl;


import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import com.fasterxml.jackson.databind.ObjectMapper;

import plus.aos.jaos.AosApi;
import plus.aos.jaos.core.common.SignArg;
import plus.aos.jaos.core.common.WalletKeyType;
import plus.aos.jaos.core.common.transaction.PackedTransaction;
import plus.aos.jaos.core.common.transaction.SignedPackedTransaction;
import plus.aos.jaos.core.request.chain.AbiJsonToBinRequest;
import plus.aos.jaos.core.request.chain.RequiredKeysRequest;
import plus.aos.jaos.core.request.chain.transaction.PushTransactionRequest;
import plus.aos.jaos.core.request.history.TransactionRequest;
import plus.aos.jaos.core.request.wallet.transaction.SignTransactionRequest;
import plus.aos.jaos.core.response.chain.AbiBinToJson;
import plus.aos.jaos.core.response.chain.AbiJsonToBin;
import plus.aos.jaos.core.response.chain.Block;
import plus.aos.jaos.core.response.chain.ChainInfo;
import plus.aos.jaos.core.response.chain.RequiredKeys;
import plus.aos.jaos.core.response.chain.TableRow;
import plus.aos.jaos.core.response.chain.abi.Abi;
import plus.aos.jaos.core.response.chain.account.Account;
import plus.aos.jaos.core.response.chain.code.Code;
import plus.aos.jaos.core.response.chain.currencystats.CurrencyStats;
import plus.aos.jaos.core.response.chain.transaction.PushedTransaction;
import plus.aos.jaos.core.response.history.action.Actions;
import plus.aos.jaos.core.response.history.controlledaccounts.ControlledAccounts;
import plus.aos.jaos.core.response.history.keyaccounts.KeyAccounts;

public class AosApiRestClientImpl implements AosApi {

    private final AosWalletApiService aosWalletApiService;

    private final AosChainApiService aosChainApiService;

    private final AosHistoryApiService aosHistoryApiService;

    public AosApiRestClientImpl(String baseUrl){
        aosWalletApiService = AosApiServiceGenerator.createService(AosWalletApiService.class, baseUrl);
        aosChainApiService = AosApiServiceGenerator.createService(AosChainApiService.class, baseUrl);
        aosHistoryApiService = AosApiServiceGenerator.createService(AosHistoryApiService.class, baseUrl);
    }

    public AosApiRestClientImpl(String baseUrl, String bearerToken){
        aosWalletApiService = AosApiServiceGenerator.createService(AosWalletApiService.class, baseUrl, bearerToken);
        aosChainApiService = AosApiServiceGenerator.createService(AosChainApiService.class, baseUrl, bearerToken);
        aosHistoryApiService = AosApiServiceGenerator.createService(AosHistoryApiService.class, baseUrl, bearerToken);
    }

    public AosApiRestClientImpl(String walletBaseUrl, String chainBaseUrl, String historyBaseUrl){
        aosWalletApiService = AosApiServiceGenerator.createService(AosWalletApiService.class, walletBaseUrl);
        aosChainApiService = AosApiServiceGenerator.createService(AosChainApiService.class, chainBaseUrl);
        aosHistoryApiService = AosApiServiceGenerator.createService(AosHistoryApiService.class, historyBaseUrl);
    }

    public AosApiRestClientImpl(String walletBaseUrl, String chainBaseUrl, String historyBaseUrl, String bearerToken){
        aosWalletApiService = AosApiServiceGenerator.createService(AosWalletApiService.class, walletBaseUrl, bearerToken);
        aosChainApiService = AosApiServiceGenerator.createService(AosChainApiService.class, chainBaseUrl, bearerToken);
        aosHistoryApiService = AosApiServiceGenerator.createService(AosHistoryApiService.class, historyBaseUrl, bearerToken);
    }

    @Override
    public ChainInfo getChainInfo(){
        return AosApiServiceGenerator.executeSync(aosChainApiService.getChainInfo());
    }

    @Override
    public Block getBlock(String blockNumberOrId){
        return AosApiServiceGenerator.executeSync(aosChainApiService.getBlock(Collections.singletonMap("block_num_or_id", blockNumberOrId)));
    }

    @Override
    public Account getAccount(String accountName){
        return AosApiServiceGenerator.executeSync(aosChainApiService.getAccount(Collections.singletonMap("account_name", accountName)));
    }

    @Override
    public Abi getAbi(String accountName){
        return AosApiServiceGenerator.executeSync(aosChainApiService.getAbi(Collections.singletonMap("account_name", accountName)));
    }

    @Override
    public Code getCode(String accountName){
        return AosApiServiceGenerator.executeSync(aosChainApiService.getCode(Collections.singletonMap("account_name", accountName)));
    }

    @Override
    public TableRow getTableRows(String scope, String code, String table){
        LinkedHashMap<String, String> requestParameters = new LinkedHashMap<>(7);

        requestParameters.put("scope", scope);
        requestParameters.put("code", code);
        requestParameters.put("table", table);
        requestParameters.put("json", "true");

        return AosApiServiceGenerator.executeSync(aosChainApiService.getTableRows(requestParameters));
    }

    @Override
    public List<String> getCurrencyBalance(String code, String accountName, String symbol){
        LinkedHashMap<String, String> requestParameters = new LinkedHashMap<>(3);

        requestParameters.put("code", code);
        requestParameters.put("account", accountName);
        requestParameters.put("symbol", symbol);

        return AosApiServiceGenerator.executeSync(aosChainApiService.getCurrencyBalance(requestParameters));
    }

    @Override
    public AbiBinToJson abiBinToJson(String code, String action, String binargs){
        LinkedHashMap<String, String> requestParameters = new LinkedHashMap<>(3);

        requestParameters.put("code", code);
        requestParameters.put("action", action);
        requestParameters.put("binargs", binargs);

        return AosApiServiceGenerator.executeSync(aosChainApiService.abiBinToJson(requestParameters));
    }

    @Override
    public <T> AbiJsonToBin abiJsonToBin(String code, String action, T args) {
        return AosApiServiceGenerator.executeSync(aosChainApiService.abiJsonToBin(new AbiJsonToBinRequest(code, action, args)));
    }

    @Override
    public PushedTransaction pushTransaction(String compression, SignedPackedTransaction packedTransaction){
        return AosApiServiceGenerator.executeSync(aosChainApiService.pushTransaction(new PushTransactionRequest(compression, packedTransaction, packedTransaction.getSignatures())));
    }
    @Override
    public PushedTransaction pushTransaction(PushTransactionRequest pushTransactionRequest) {
        return AosApiServiceGenerator.executeSync(aosChainApiService.pushTransaction(pushTransactionRequest));
    }

    @Override
    public List<PushedTransaction> pushTransactions(List<PushTransactionRequest> pushTransactionRequests){
        return AosApiServiceGenerator.executeSync(aosChainApiService.pushTransactions(pushTransactionRequests));
    }

    @Override
    public RequiredKeys getRequiredKeys(PackedTransaction transaction, List<String> keys){
        return AosApiServiceGenerator.executeSync(aosChainApiService.getRequiredKeys(new RequiredKeysRequest(transaction, keys)));
    }

    @Override
    public Map<String, CurrencyStats> getCurrencyStats(String code, String symbol){
        LinkedHashMap<String, String> requestParameters = new LinkedHashMap<>(2);

        requestParameters.put("code", code);
        requestParameters.put("symbol", symbol);

        return AosApiServiceGenerator.executeSync(aosChainApiService.getCurrencyStats(requestParameters));
    }

    @Override
    public String createWallet(String walletName){
        return AosApiServiceGenerator.executeSync(aosWalletApiService.createWallet(walletName));
    }

    @Override
    public void openWallet(String walletName){
        AosApiServiceGenerator.executeSync(aosWalletApiService.openWallet(walletName));
    }

    @Override
    public void lockWallet(String walletName){
        AosApiServiceGenerator.executeSync(aosWalletApiService.lockWallet(walletName));
    }

    @Override
    public void lockAllWallets(){
        AosApiServiceGenerator.executeSync(aosWalletApiService.lockAll());
    }

    @Override
    public void unlockWallet(String walletName, String walletPassword){
        List<String> requestFields = new ArrayList<>(2);

        requestFields.add(walletName);
        requestFields.add(walletPassword);
        AosApiServiceGenerator.executeSync(aosWalletApiService.unlockWallet(requestFields));
    }

    @Override
    public void importKeyIntoWallet(String walletName, String key){
        List<String> requestFields = new ArrayList<>(2);

        requestFields.add(walletName);
        requestFields.add(key);
        AosApiServiceGenerator.executeSync(aosWalletApiService.importKey(requestFields));
    }

    @Override
    public List<String> listWallets(){
        return AosApiServiceGenerator.executeSync(aosWalletApiService.listWallets());
    }

    @Override
    public List<List<String>> listKeys(String walletName, String password){
        List<String> requestFields = Arrays.asList(walletName, password);
       return AosApiServiceGenerator.executeSync(aosWalletApiService.listKeys(requestFields));
    }

    @Override
    public List<String> getPublicKeys(){
        return AosApiServiceGenerator.executeSync(aosWalletApiService.getPublicKeys());
    }

    @Override
    public SignedPackedTransaction signTransaction(PackedTransaction packedTransaction, List<String> publicKeys, String chainId) {
        return AosApiServiceGenerator.executeSync(aosWalletApiService.signTransaction(new SignTransactionRequest(packedTransaction, publicKeys, chainId)));
    }

    @Override
    public void setWalletTimeout(Integer timeout){
        AosApiServiceGenerator.executeSync(aosWalletApiService.setTimeout(timeout));
    }

    @Override
    public String signDigest(String digest, String publicKey){
        return AosApiServiceGenerator.executeSync(aosWalletApiService.signDigest(Arrays.asList(digest, publicKey)));
    }

    @Override
    public String createKey(String walletName, WalletKeyType keyType){
        return AosApiServiceGenerator.executeSync(aosWalletApiService.createKey(Arrays.asList(walletName, keyType.name())));
    }

    @Override
    public Actions getActions(String accountName, Integer pos, Integer offset){
        LinkedHashMap<String, Object> requestParameters = new LinkedHashMap<>(3);

        requestParameters.put("account_name", accountName);
        requestParameters.put("pos", pos);
        requestParameters.put("offset", offset);

        return AosApiServiceGenerator.executeSync(aosHistoryApiService.getActions(requestParameters));
    }

    @Override
    public Object getTransaction(TransactionRequest transactionRequest){
        return AosApiServiceGenerator.executeSync(aosHistoryApiService.getTransaction(transactionRequest));
    }

    @Override
    public KeyAccounts getKeyAccounts(String publicKey){
        LinkedHashMap<String, String> requestParameters = new LinkedHashMap<>(1);

        requestParameters.put("public_key", publicKey);

        return AosApiServiceGenerator.executeSync(aosHistoryApiService.getKeyAccounts(requestParameters));
    }

    @Override
    public ControlledAccounts getControlledAccounts(String controllingAccountName){
        LinkedHashMap<String, String> requestParameters = new LinkedHashMap<>(1);

        requestParameters.put("controlling_account", controllingAccountName);

        return AosApiServiceGenerator.executeSync(aosHistoryApiService.getControlledAccounts(requestParameters));
    }

    @Override
    public ObjectMapper getObjectMapper() {
        return AosApiServiceGenerator.getMapper();
    }
    
    @Override
    public SignArg getSignArg(int expiredSecond) {
        ChainInfo info = getChainInfo();
        Block block = getBlock(info.getLastIrreversibleBlockNum().toString());
        
        SignArg arg = new SignArg();
        arg.setChainId(info.getChainId());
        arg.setExpiredSecond(expiredSecond);
        arg.setHeadBlockNum(info.getHeadBlockNum());
        arg.setHeadBlockTime(info.getHeadBlockTime());
        arg.setLastIrreversibleBlockNum(info.getLastIrreversibleBlockNum());
        arg.setRefBlockPrefix(block.getRefBlockPrefix());
        return arg;
    }
    
    // ------------------------------------------------------------------------------
    //                                                                              //
    //                                  LOCAL API                                   //
    //                                                                              //
    // ------------------------------------------------------------------------------
    
}
