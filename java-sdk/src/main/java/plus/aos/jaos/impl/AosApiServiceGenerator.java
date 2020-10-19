package plus.aos.jaos.impl;

import java.io.IOException;
import java.lang.annotation.Annotation;

import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.PropertyNamingStrategy.SnakeCaseStrategy;
import com.fasterxml.jackson.databind.SerializationFeature;

import plus.aos.jaos.exception.AosApiError;
import plus.aos.jaos.exception.AosApiErrorCode;
import plus.aos.jaos.exception.AosApiException;
import okhttp3.Interceptor;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import retrofit2.Call;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.jackson.JacksonConverterFactory;

public class AosApiServiceGenerator {

    private static OkHttpClient httpClient;

    private static Retrofit retrofit;
    private static ObjectMapper mapper = new ObjectMapper();
    static {
        mapper.disable(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES);
        mapper.configure(SerializationFeature.INDENT_OUTPUT, true);
        mapper.configure(SerializationFeature.WRITE_DATES_AS_TIMESTAMPS, false);
        mapper.setPropertyNamingStrategy(new SnakeCaseStrategy());
        mapper.findAndRegisterModules();
        //
        httpClient = new OkHttpClient.Builder().addInterceptor(new LoggingInterceptor()).build();

    }

    public static ObjectMapper getMapper() {
        return mapper;
    }

    public static <S> S createService(Class<S> serviceClass, String baseUrl) {
        Retrofit.Builder builder = new Retrofit.Builder();
        builder.baseUrl(baseUrl);
        builder.client(httpClient);
        builder.addConverterFactory(JacksonConverterFactory.create(mapper));
        retrofit = builder.build();

        return retrofit.create(serviceClass);
    }

    public static <S> S createService(Class<S> serviceClass, String baseUrl, String bearerToken) {
    	OkHttpClient httpClientWithToken = new OkHttpClient.Builder()
    			.addInterceptor(new Interceptor() {
    			      @Override
    			      public okhttp3.Response intercept(Chain chain) throws IOException {
    			        Request newRequest  = chain.request().newBuilder()
    			            .addHeader("Authorization", "Bearer " + bearerToken)
    			            .build();
    			        return chain.proceed(newRequest);
    			      }
    			    })
    			.addInterceptor(new LoggingInterceptor())
    			.build();
    	
        Retrofit.Builder builder = new Retrofit.Builder();
        builder.baseUrl(baseUrl);
        builder.client(httpClientWithToken);
        builder.addConverterFactory(JacksonConverterFactory.create(mapper));
        retrofit = builder.build();

        return retrofit.create(serviceClass);
    }

    /**
     * Execute a REST call and block until the response is received.
     */
    public static <T> T executeSync(Call<T> call) {
        try {
            Response<T> response = call.execute();
            if (response.isSuccessful()) {
                return response.body();
            } else {
                AosApiError apiError = getAosApiError(response);
                throw new AosApiException(apiError.getDetailedMessage(), AosApiErrorCode.get(apiError.getAosErrorCode()));
            }
        } catch (IOException e) {
            throw new AosApiException(e);
        }
    }

    /**
     * Extracts and converts the response error body into an object.
     */
    private static AosApiError getAosApiError(Response<?> response) throws IOException, AosApiException {
        return (AosApiError) retrofit.responseBodyConverter(AosApiError.class, new Annotation[0]).convert(response.errorBody());
    }
}
