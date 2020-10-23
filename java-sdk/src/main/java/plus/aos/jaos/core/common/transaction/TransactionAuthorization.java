package plus.aos.jaos.core.common.transaction;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class TransactionAuthorization {

    private String actor;
    private String permission;
}
