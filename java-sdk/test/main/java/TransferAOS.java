import plus.aos.jaos.AosApi;
import plus.aos.jaos.AosApiFactory;
import plus.aos.jaos.LocalApi;
import plus.aos.jaos.core.common.SignArg;
import plus.aos.jaos.core.request.chain.transaction.PushTransactionRequest;
import plus.aos.jaos.core.response.chain.transaction.PushedTransaction;


public class TransferAOS {

    public static void main(String[] args) throws Exception {

        // --- get the current state of blockchain
        AosApi aosApi = AosApiFactory.create("http://api.aos.plus:8888");

        // -- set expired seconds
        SignArg arg = aosApi.getSignArg(120);
        System.out.println(aosApi.getObjectMapper().writeValueAsString(arg));

        // get current account's balance
        System.out.println(aosApi.getCurrencyBalance("aosio.token", "aweoswallet1", "AOS"));

        // --- sign the transaction of token transfer
        String privateKey = "5JDGy4o9mfHHB81CxuU5RwrrFxuZjZbDaSQiPLhipXmDRcDmVUM"; //replace the real private key
        String from = "aweoswallet1";  // from account
        String to = "madajiasijia"; // to account
        String quantity = "0.0001 AOS"; //value
        String memo = "sent by aos sdk"; //momeo info
        LocalApi localApi = AosApiFactory.createLocalApi();
        PushTransactionRequest req = localApi.transfer(arg, privateKey, from, to, quantity, memo);

        System.out.println(localApi.getObjectMapper().writeValueAsString(req));

        // --- push the signed-transaction to the blockchain
        PushedTransaction pts = aosApi.pushTransaction(req);
        System.out.println(localApi.getObjectMapper().writeValueAsString(pts));

    }
}