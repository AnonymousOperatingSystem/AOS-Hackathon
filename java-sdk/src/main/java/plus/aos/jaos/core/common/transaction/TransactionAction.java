package plus.aos.jaos.core.common.transaction;


import java.util.List;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class TransactionAction {

    private String account;

    private String name;

    private List<TransactionAuthorization> authorization;

    private String data;

}
