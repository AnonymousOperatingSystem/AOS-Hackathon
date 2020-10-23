package plus.aos.jaos.exception;

public class AosApiError {

    private String message;

    private int code;

    private AosError error;

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }

    public AosError getError() {
        return error;
    }

    public void setError(AosError error) {
        this.error = error;
    }

    

    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder();
        builder.append("AosApiError [message=");
        builder.append(message);
        builder.append(", code=");
        builder.append(code);
        builder.append(", error=");
        builder.append(error);
        builder.append("]");
        return builder.toString();
    }

    public String getDetailedMessage() {
        return error == null ? message : message + ": " + error.getWhat();
    }

    public Integer getAosErrorCode() {
        return error == null ? null : error.getCode();
    }
}
