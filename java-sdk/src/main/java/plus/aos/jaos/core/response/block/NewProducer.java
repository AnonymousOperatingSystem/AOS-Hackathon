package plus.aos.jaos.core.response.block;

import java.util.List;

import lombok.Data;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
public class NewProducer {
    private String version;
    private List<Producer> producers;
}
