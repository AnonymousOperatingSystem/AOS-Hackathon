package plus.aos.jaos.core.request.chain.transaction;

import java.util.List;

import plus.aos.jaos.core.common.transaction.PackedTransaction;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class PushTransactionRequest {

    private String compression = "none";
    private PackedTransaction transaction;
    private List<String> signatures;

}
