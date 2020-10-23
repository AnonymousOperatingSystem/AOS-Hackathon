package plus.aos.jaos.impl;

import java.util.Map;

import plus.aos.jaos.core.request.history.TransactionRequest;
import plus.aos.jaos.core.response.history.action.Actions;
import plus.aos.jaos.core.response.history.controlledaccounts.ControlledAccounts;
import plus.aos.jaos.core.response.history.keyaccounts.KeyAccounts;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.POST;

public interface AosHistoryApiService {

    @POST("/v1/history/get_actions")
    Call<Actions> getActions(@Body Map<String, Object> requestFields);

    @POST("/v1/history/get_transaction")
    Call<Object> getTransaction(@Body TransactionRequest transactionRequest);

    @POST("/v1/history/get_key_accounts")
    Call<KeyAccounts> getKeyAccounts(@Body Map<String, String> requestFields);

    @POST("/v1/history/get_controlled_accounts")
    Call<ControlledAccounts> getControlledAccounts(@Body Map<String, String> requestFields);

}
