package plus.aos.jaos.core.response.history.transaction;

import java.io.IOException;
import java.util.Optional;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.deser.std.StdDeserializer;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
public class TrxDeserializer extends StdDeserializer<Optional<Trx>> {

    public TrxDeserializer() {
        super(Trx.class);
    }

    @Override
    public Optional<Trx> deserialize(JsonParser p, DeserializationContext ctxt) throws IOException, JsonProcessingException {
        return Optional.ofNullable(p.isExpectedStartObjectToken() ? p.readValueAs(Trx.class) : null);
    }
}
