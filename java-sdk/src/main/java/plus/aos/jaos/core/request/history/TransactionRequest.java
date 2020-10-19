package plus.aos.jaos.core.request.history;

import lombok.Data;
import lombok.NonNull;
import lombok.RequiredArgsConstructor;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
@RequiredArgsConstructor
public class TransactionRequest {
    
    @NonNull
    private String id;
    private Long blockNumHint;
}
