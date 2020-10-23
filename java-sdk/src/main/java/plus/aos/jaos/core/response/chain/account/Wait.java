package plus.aos.jaos.core.response.chain.account;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class Wait {

    private Long weightSec;
    private Integer weight;
}
