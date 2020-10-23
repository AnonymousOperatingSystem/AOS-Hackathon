package plus.aos.jaos;

import java.util.List;
import java.util.Map;

import com.fasterxml.jackson.databind.ObjectMapper;

import plus.aos.jaos.core.common.SignArg;
import plus.aos.jaos.core.common.WalletKeyType;
import plus.aos.jaos.core.common.transaction.PackedTransaction;
import plus.aos.jaos.core.common.transaction.SignedPackedTransaction;
import plus.aos.jaos.core.request.chain.transaction.PushTransactionRequest;
import plus.aos.jaos.core.request.history.TransactionRequest;
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

public interface AosApi{

    String CHAIN_ID_MAINET = "aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906";
    String CHAIN_ID_JUNGLE = "038f4b0fc8ff18a4f0842a8f0564611f6e96e8535901dd45e43ac8691a1c4dca";

    /**
     *
     * @return
     */
    ChainInfo getChainInfo();

    Block getBlock(String blockNumberOrId);

    Account getAccount(String accountName);

    Abi getAbi(String accountName);

    Code getCode(String accountName);

    TableRow getTableRows(String scope, String code, String table);

    List<String> getCurrencyBalance(String code, String accountName, String symbol);

    AbiBinToJson abiBinToJson(String code, String action, String binargs);

    <T> AbiJsonToBin abiJsonToBin(String code, String action, T args);

    PushedTransaction pushTransaction(String compression, SignedPackedTransaction packedTransaction);
    PushedTransaction pushTransaction(PushTransactionRequest pushTransactionRequest);
    
    List<PushedTransaction> pushTransactions(List<PushTransactionRequest> pushTransactionRequests);

    RequiredKeys getRequiredKeys(PackedTransaction transaction, List<String> keys);

    Map<String, CurrencyStats> getCurrencyStats(String code, String symbol);

    String createWallet(String walletName);

    void openWallet(String walletName);

    void lockWallet(String walletName);

    void lockAllWallets();

    void unlockWallet(String walletName, String walletPassword);

    void importKeyIntoWallet(String walletName, String walletKey);

    List<String> listWallets();

    List<List<String>> listKeys(String walletName, String password);

    List<String> getPublicKeys();

    SignedPackedTransaction signTransaction(PackedTransaction unsignedTransaction, List<String> publicKeys, String chainId);

    void setWalletTimeout(Integer timeout);

    String signDigest(String digest, String publicKey);

    String createKey(String walletName, WalletKeyType walletKeyType);

    Actions getActions(String accountName, Integer pos, Integer offset);
    
    //TODO: changed day by day
    Object getTransaction(TransactionRequest transactionRequest);

    KeyAccounts getKeyAccounts(String publicKey);

    ControlledAccounts getControlledAccounts(String controllingAccountName);

    ObjectMapper getObjectMapper();
    
    SignArg getSignArg(int expiredSecond);
}
