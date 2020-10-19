package plus.aos.jaos.core.request.chain.json2bin;

import plus.aos.jaos.core.response.chain.account.RequiredAuth;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
public class CreateAccountArg{

    private String creator;
    private String name;
    private RequiredAuth owner;
    private RequiredAuth active;

}
